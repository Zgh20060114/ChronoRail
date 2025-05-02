cmake_minimum_required(VERSION 3.10)
project(ChronoRail VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SOURCES "src/*.cpp")

add_executable(chronorail ${SOURCES})


target_link_libraries(chronorail) 
