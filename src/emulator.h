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
    void update();
    void draw();
private:
    void draw_cpu(i32 x, i32 y);
    void draw_ram(i32 x, i32 y, u16 addr, u16 rows, u16 cols);
    void draw_code(i32 x, i32 y, u16 addr, u16 rows, u16 cols);

    mem_bus nes;
};



#endif //EMULATOR_H
