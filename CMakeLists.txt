cmake_minimum_required(VERSION 3.28)
project(kap_nes_emu)

set(CMAKE_CXX_STANDARD 17)

add_executable(kap_nes_emu src/main.cpp
        src/defines.h
        src/logger.h
        src/bus.cpp
        src/bus.h
        src/cpu.cpp
        src/cpu.h)
