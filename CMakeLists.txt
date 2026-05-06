cmake_minimum_required(VERSION 3.20)
project(warehouse VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SQLite3 REQUIRED)

file(GLOB_RECURSE SOURCES
    "db/*.cpp"
    "Models/*.cpp"
    "repositories/*.cpp"
    "handlers/*.cpp"
    "app/*.cpp"
    "main.cpp"
)

add_executable(warehouse ${SOURCES})

target_include_directories(warehouse PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${SQLite3_INCLUDE_DIRS}
)

target_link_libraries(warehouse PRIVATE
    SQLite3::SQLite3
)
