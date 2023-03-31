cmake_minimum_required(VERSION 2.8.9)
project(energy_monitor)

find_package(Curses REQUIRED)

include_directories(include)
include_directories(${CURSES_INCLUDE_DIR})
# file(GLOB SOURCES "src/*.cpp")

add_executable(energy_monitor 
               src/main.cpp 
               src/system_parser.cpp 
               src/memory.cpp 
               src/process.cpp 
               src/processor.cpp 
               src/system.cpp 
               src/power.cpp 
               src/display.cpp 
               src/format.cpp
               src/date_time.cpp
               src/power_dao.cpp)
target_link_libraries(energy_monitor ${CURSES_LIBRARIES})