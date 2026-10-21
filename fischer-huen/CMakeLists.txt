cmake_minimum_required(VERSION 3.16)

project(fischer-huen LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME}
	src/implementation.cpp
)

target include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src/headers
)
