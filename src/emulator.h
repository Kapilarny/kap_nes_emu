//
// Created by user on 06.07.2024.
//

#ifndef EMULATOR_H
#define EMULATOR_H
#include "cpu6502.h"
#include "defines.h"
#include "mem_bus.h"


class emulator {
public:
    emulator();

    void update();
    void draw();
private:
    void draw_cpu(i32 x, i32 y);
    void draw_ram(i32 x, i32 y, u16 addr, u16 rows, u16 cols);
    void draw_code(i32 x, i32 y, i32 lines);

    void draw_sprite(i32 x, i32 y, const sprite& spr, i32 scale = 1);

    std::map<u16, std::string> mapAsm;
    mem_bus nes;
    std::shared_ptr<cartridge> cart;

    bool emulation_run = false;
    f32 residual_time = 0.0f;
};



#endif //EMULATOR_H
