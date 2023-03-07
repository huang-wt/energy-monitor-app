cmake_minimum_required(VERSION 2.8.9)
project(energy_monitor)

include_directories(include)
# file(GLOB SOURCES "src/*.cpp")

add_executable(energy_monitor src/main.cpp src/system_parser.cpp src/memory.cpp src/process.cpp src/processor.cpp src/system.cpp src/power.cpp src/view.cpp)