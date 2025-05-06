cmake_minimum_required(VERSION 3.10...3.27)

set(CMAKE_C_COMPILER /opt/homebrew/bin/gcc)
set(CMAKE_CXX_COMPILER /opt/homebrew/bin/g++)
project(minimal_logging)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(PROJECT_SOURCE_DIR ./src)

add_compile_definitions( SOURCE_ROOT="${CMAKE_SOURCE_DIR}")


include_directories("/usr/include/opencv4/")
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})
include_directories(/opt/homebrew/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/cereal/include)

add_definitions(-DPRJ_DIR="${CMAKE_CURRENT_SOURCE_DIR}/")

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp ${PROJECT_SOURCE_DIR}/*.h)

add_executable(minimal_logging ../main.cpp)

target_include_directories(minimal_logging PUBLIC ${PROJECT_SOURCE_DIR}/src)
# target_link_libraries(minimal_logging )


