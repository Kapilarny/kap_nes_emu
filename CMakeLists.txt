cmake_minimum_required(VERSION 3.28)
project(kap_nes_emu)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external/raylib)

add_executable(kap_nes_emu src/main.cpp
        src/defines.h
        src/logger.h
        src/mem_bus.cpp
        src/mem_bus.h
        src/cpu6502.cpp
        src/cpu6502.h
        src/ppu.cpp
        src/ppu.h
        src/cartridge.cpp
        src/cartridge.h
        src/mappers/mapper.cpp
        src/mappers/mapper.h
        src/mappers/impl/mapper_000.cpp
        src/mappers/impl/mapper_000.h
        src/gamepad.cpp
        src/gamepad.h)

target_link_libraries(kap_nes_emu PRIVATE -static opengl32 user32 gdi32 gdiplus SHlwapi dwmapi xinput)
target_include_directories(kap_nes_emu PRIVATE src external/olcPixelEngine)
