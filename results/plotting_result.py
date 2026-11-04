import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from matplotlib.ticker import FixedLocator, FuncFormatter, NullLocator

import sys
import argparse
import re
from pathlib import Path

IMPLS = {
    "Naive_Linear": ("Naive linear", "#7f7f7f", "o"),
    "Segment_Tree": ("Segment tree", "#2ca02c", "s"),
    "Sparse_Table": ("Sparse table", "#1f77b4", "^"),
    "Fischer_Heun": ("Fischer\u2013Heun", "#d62728", "D"),
    "Full_Table": ("Naive block (Full_Table)", "#ff7f0e", "v"),
}
ORDER = list(IMPLS)
UNIT_TO_US = {"ns": 1e-3, "us": 1.0, "ms": 1e3, "s": 1e6}
KEYS = ["bench", "impl", "pattern", "n"]
 
AGG_RE = re.compile(r"_(mean|median|stddev|cv)$")
NAME_RE = re.compile(
    r"^BM_(?P<bench>\w+?)<(?P<impl>[\w:]+)>/(?P<pattern>\w+)"
    r"(?P<rest>(?:/[\w.]+(?::[\w.]+)?)*)$"
)
 
 
# --------------------------------------------------------------------------
# Loading
# --------------------------------------------------------------------------
def load(path, time_col):
    lines = Path(path).read_text(encoding="utf-8-sig").splitlines()
    start = next((i for i, l in enumerate(lines) if l.startswith("name,")), None)
    if start is None:
        sys.exit("Could not find the CSV header line (it should start with 'name,').")
    df = pd.read_csv(path, skiprows=start, encoding="utf-8-sig")
    df = df.rename(columns={"bytes/elem": "bytes_per_elem"})
 
    if "error_occurred" in df:
        df = df[~df["error_occurred"].astype(str).str.lower().eq("true")]
 
    agg = df["name"].str.extract(AGG_RE)[0].rename("agg")
    parsed = df["name"].str.replace(AGG_RE, "", regex=True).str.extract(NAME_RE)
    bad = parsed["impl"].isna()
    if bad.any():
        print(f"[warn] ignoring {bad.sum()} rows with unrecognised names, "
              f"e.g. {df.loc[bad, 'name'].iloc[0]!r}")
    df = pd.concat([df, parsed, agg], axis=1)[~bad].copy()
 
    df["impl"] = df["impl"].str.split("::").str[-1]  # drop any namespace prefix
    df["n"] = pd.to_numeric(df["rest"].str.extract(r"/n:(\d+)")[0])
    df = df.dropna(subset=["n"])
    df["n"] = df["n"].astype(int)
    df["time_us"] = df[time_col] * df["time_unit"].map(UNIT_TO_US)
    return df
 
 
def summarise(df):
    """One row per (bench, impl, pattern, n): median time, min/max, bytes/elem."""
    raw = df[df["agg"].isna()]
    if raw.empty:  # --benchmark_report_aggregates_only was used
        raw = df[df["agg"] == "median"]
    spec = dict(
        t_med=("time_us", "median"),
        t_lo=("time_us", "min"),
        t_hi=("time_us", "max"),
        reps=("time_us", "size"),
    )
    if "bytes_per_elem" in raw:
        spec["bpe"] = ("bytes_per_elem", "median")
    res = raw.groupby(KEYS).agg(**spec).reset_index()
    res["ns_per_elem"] = res["t_med"] * 1e3 / res["n"]
    return res
 
 
def wide(res, pattern, col):
    """Table with n as rows and one column per implementation."""
    d = res[res["pattern"] == pattern]
    return d.pivot(index="n", columns="impl", values=col).sort_index()
 
 
# --------------------------------------------------------------------------
# Plot helpers
# --------------------------------------------------------------------------
def style_of(impl):
    return IMPLS.get(impl, (impl, "black", "x"))
 
 
def pow2_fmt(v, _):
    k = np.log2(v)
    return f"$2^{{{int(round(k))}}}$" if abs(k - round(k)) < 1e-9 else ""
 
 
def times_axis(ax):
    """Label a log y-axis as 1x, 2x, 0.5x ... (major and minor ticks)."""
    fmt = FuncFormatter(lambda v, _: f"{v:g}\u00d7")
    ax.yaxis.set_major_formatter(fmt)
    ax.yaxis.set_minor_formatter(fmt)
 
 
def setup(ax, res, ylabel, logy=True, title=None):
    ks = range(int(np.floor(np.log2(res["n"].min()))),
               int(np.ceil(np.log2(res["n"].max()))) + 1)
    ax.set_xscale("log", base=2)
    if logy:
        ax.set_yscale("log")
    ax.xaxis.set_major_locator(FixedLocator([2 ** k for k in ks if (k - ks[0]) % 2 == 0]))
    ax.xaxis.set_major_formatter(FuncFormatter(pow2_fmt))
    ax.xaxis.set_minor_locator(NullLocator())
    ax.set_xlabel("array size n")
    ax.set_ylabel(ylabel)
    ax.grid(True, which="major", alpha=0.3)
    if title:
        ax.set_title(title, fontsize=11)
 
 
def draw(ax, res, impl, pattern, col, band=False, **kw):
    d = res[(res["impl"] == impl) & (res["pattern"] == pattern)].sort_values("n")
    if d.empty:
        return None
    label, colour, marker = style_of(impl)
    kw.setdefault("label", label)
    ax.plot(d["n"], d[col], marker=marker, ms=4.5, lw=1.6, color=colour, **kw)
    if band and (d["t_hi"] > d["t_lo"]).any():
        ax.fill_between(d["n"], d["t_lo"], d["t_hi"], color=colour, alpha=0.18, lw=0)
    return d
 
 
def block_crossover(w, block="Full_Table"):
    """(previous n, n*): the smallest measured n from which the naive block is the
    slowest build at every larger measured n. previous n is None if it is the
    slowest everywhere."""
    if block not in w:
        return None
    ns = w[block].dropna().index
    is_slowest = (w.loc[ns].idxmax(axis=1) == block).to_numpy()
    for i in range(len(ns)):
        if is_slowest[i:].all():
            return (ns[i - 1] if i > 0 else None), ns[i]
    return None
 
 
# --------------------------------------------------------------------------
# Figures
# --------------------------------------------------------------------------
def fig_build_time(res, pattern):
    """#5: absolute build time, with the point where naive block becomes slowest."""
    w = wide(res, pattern, "t_med")
    fig, ax = plt.subplots(figsize=(8, 5), constrained_layout=True)
    for impl in ORDER:
        draw(ax, res, impl, pattern, "t_med", band=True)
 
    if "Full_Table" in w:  # n^2 guide line through the last naive-block point
        d = w["Full_Table"].dropna()
        xs = d.index.to_numpy(float)
        ax.plot(xs, d.iloc[-1] * (xs / xs[-1]) ** 2, "k:", lw=1, label=r"$\propto n^2$ guide")
 
    setup(ax, res, "build time (\u00b5s)",
          title=f"Build time vs n ({pattern} input)")
    cross = block_crossover(w)
    if cross:
        prev, n_star = cross
        if prev:
            ax.axvspan(prev, n_star, color=IMPLS["Full_Table"][1], alpha=0.12, lw=0)
            msg = f"naive block becomes slowest\nbetween n={prev:,} and n={n_star:,}"
            x = np.sqrt(prev * n_star)
        else:
            msg, x = f"naive block slowest at every\nmeasured n (from n={n_star:,})", n_star
        ax.text(x, 0.97, msg, transform=ax.get_xaxis_transform(),
                ha="center", va="top", fontsize=8)
    ax.legend(frameon=False, fontsize=9, loc="lower right")
    return fig
 
 
def fig_per_element(res, pattern):
    """#11: time per element. Flat = linear-time build; rising = super-linear."""
    fig, ax = plt.subplots(figsize=(8, 5), constrained_layout=True)
    for impl in ORDER:
        draw(ax, res, impl, pattern, "ns_per_elem")
    setup(ax, res, "build time per element (ns)",
          title=f"Build cost per element ({pattern} input): flat line = linear-time build")
    ax.legend(frameon=False, fontsize=9)
    return fig
 
 
def fig_ratios(res, pattern, num="Fischer_Heun", dens=("Segment_Tree", "Sparse_Table")):
    """#11: how many times slower Fischer-Heun builds than the other structures."""
    w = wide(res, pattern, "t_med")
    fig, ax = plt.subplots(figsize=(8, 4.6), constrained_layout=True)
    for den in dens:
        if num in w and den in w:
            r = (w[num] / w[den]).dropna()
            ax.plot(r.index, r, marker=style_of(den)[2], ms=5, lw=1.6, color=style_of(den)[1],
                    label=f"{style_of(num)[0]} \u00f7 {style_of(den)[0].lower()}")
    ax.axhline(1, color="k", ls="--", lw=1)
    setup(ax, res, "build-time ratio (>1: Fischer\u2013Heun slower)",
          title=f"Fischer\u2013Heun build time relative to the others ({pattern} input)")
    times_axis(ax)
    ax.legend(frameon=False, fontsize=9)
    return fig
 
 
def fig_memory(res, pattern):
    """#7: bytes per input element, as reported by the benchmark's own counter."""
    if "bpe" not in res:
        return None
    fig, ax = plt.subplots(figsize=(8, 5), constrained_layout=True)
    for impl in ORDER:
        draw(ax, res, impl, pattern, "bpe")
    setup(ax, res, "bytes per input element",
          title=f"Structure size per element ({pattern} input)")
    ax.legend(frameon=False, fontsize=9)
    fig.text(0.01, 0.005, "Memory = the benchmark's own `bytes` counter.", fontsize=7, color="gray")
    return fig
 
 
def fig_pattern(res, a="random", b="ascending", impl="Fischer_Heun"):
    """Input-pattern sensitivity: random vs ascending arrays."""
    pats = set(res["pattern"])
    if not {a, b} <= pats:
        print(f"[skip] fig5: needs both '{a}' and '{b}' patterns, found {sorted(pats)}")
        return None
    wa, wb = wide(res, a, "t_med"), wide(res, b, "t_med")
    has_mem = "bpe" in res
    fig, axes = plt.subplots(1, 2 if has_mem else 1, figsize=(12 if has_mem else 7, 4.6),
                             constrained_layout=True, squeeze=False)
    ax = axes[0, 0]
    for i in ORDER:
        if i in wa and i in wb:
            r = (wa[i] / wb[i]).dropna()
            ax.plot(r.index, r, marker=style_of(i)[2], ms=4.5, lw=1.4,
                    color=style_of(i)[1], label=style_of(i)[0])
    ax.axhline(1, color="k", ls="--", lw=1)
    setup(ax, res, f"build time: {a} \u00f7 {b}", title="Does the input pattern change build time?")
    times_axis(ax)
    ax.legend(frameon=False, fontsize=8)
 
    if has_mem:
        ax = axes[0, 1]
        for pat, ls in ((a, "-"), (b, "--")):
            draw(ax, res, impl, pat, "bpe", label=f"{style_of(impl)[0]}, {pat}", ls=ls)
        setup(ax, res, "bytes per input element", logy=False,
              title=f"{style_of(impl)[0]}: structure size by input pattern")
        ax.legend(frameon=False, fontsize=8)
    return fig
 
 
# --------------------------------------------------------------------------
# Console summary: the numbers behind the figures, handy for writing the report
# --------------------------------------------------------------------------
def print_findings(res, pattern):
    w = wide(res, pattern, "t_med")
    print(f"\n=== Build time ({pattern} input), cpu median in \u00b5s ===")
    print("n, slowest and fastest build among the implementations measured at that n:")
    for n, row in w.iterrows():
        print(f"  n={n:>10,}  slowest: {row.idxmax():<13} fastest: {row.idxmin():<13}")
    cross = block_crossover(w)
    if cross:
        prev, n_star = cross
        print(f"\n#5 naive block becomes the slowest build "
              f"{'between n=%s and ' % format(prev, ',') if prev else 'from '}n={n_star:,} "
              f"(measured up to n={w['Full_Table'].dropna().index.max():,}).")
    else:
        print("\n#5 naive block never settles as the slowest build in the measured range.")
    if "Fischer_Heun" in w:
        for den in ("Segment_Tree", "Sparse_Table"):
            if den in w:
                r = (w["Fischer_Heun"] / w[den]).dropna()
                print(f"#11 Fischer-Heun / {den}: min {r.min():.2f}x (n={r.idxmin():,}), "
                      f"max {r.max():.2f}x (n={r.idxmax():,}), at largest n {r.iloc[-1]:.2f}x")
    if "bpe" in res:
        wb = wide(res, pattern, "bpe")
        print(f"\n=== Memory ordering ({pattern} input), bytes/element, largest to smallest ===")
        for n, row in wb.iterrows():
            order = " > ".join(row.dropna().sort_values(ascending=False).index)
            print(f"  n={n:>10,}  {order}")
    print("\nLargest n measured per implementation:")
    print(res[res["pattern"] == pattern].groupby("impl")["n"].max().to_string())
    if res["reps"].max() == 1:
        print("\n[note] every point is a single run: run the benchmark with "
              "--benchmark_repetitions=5 to get error bands.")
 
 
# --------------------------------------------------------------------------
def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("csv", help="Google Benchmark CSV file")
    ap.add_argument("--out", default="figures", help="output directory (default: figures)")
    ap.add_argument("--bench", default="Build", help="benchmark family to plot (default: Build)")
    ap.add_argument("--pattern", default="random", help="input pattern for the main figures")
    ap.add_argument("--time-col", default="cpu_time", choices=["cpu_time", "real_time"])
    ap.add_argument("--formats", nargs="+", default=["png", "pdf"])
    ap.add_argument("--show", action="store_true", help="also open the figures in a window")
    args = ap.parse_args()
 
    plt.rcParams.update({"font.size": 10, "axes.spines.top": False, "axes.spines.right": False})
 
    df = load(args.csv, args.time_col)
    res = summarise(df)
    benches = sorted(res["bench"].unique())
    res = res[res["bench"] == args.bench]
    if res.empty:
        sys.exit(f"No rows for bench '{args.bench}'. Found: {benches}")
    if args.pattern not in set(res["pattern"]):
        sys.exit(f"Pattern '{args.pattern}' not found. Found: {sorted(res['pattern'].unique())}")
 
    print_findings(res, args.pattern)
 
    out = Path(args.out)
    out.mkdir(parents=True, exist_ok=True)
    figs = {
        "fig1_build_time": fig_build_time(res, args.pattern),
        "fig2_build_per_element": fig_per_element(res, args.pattern),
        "fig3_build_ratios": fig_ratios(res, args.pattern),
        "fig4_memory": fig_memory(res, args.pattern),
        "fig5_pattern_sensitivity": fig_pattern(res),
    }
    for name, fig in figs.items():
        if fig is None:
            continue
        for ext in args.formats:
            fig.savefig(out / f"{name}.{ext}", dpi=200)
    print(f"\nSaved {sum(f is not None for f in figs.values())} figures to {out.resolve()}")
    if args.show:
        plt.show()
 
 
if __name__ == "__main__":
    main()