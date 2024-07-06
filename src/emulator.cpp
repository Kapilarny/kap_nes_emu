//
// Created by user on 06.07.2024.
//

#include "emulator.h"

#include <string>

#include "raylib.h"

void DrawString(std::string str, i32 x, i32 y, i32 size, Color color) {
    DrawText(str.c_str(), x, y, size, color);
}

std::string hex(u32 n, u8 d)
{
    std::string s(d, '0');
    for (int i = d - 1; i >= 0; i--, n >>= 4)
        s[i] = "0123456789ABCDEF"[n & 0xF];
    return s;
};

void emulator::update() {

}

void emulator::draw() {
}

void emulator::draw_cpu(i32 x, i32 y) {
    std::string status = "STATUS: ";
    DrawText(status.c_str(), x, y, 10, WHITE);
    DrawText("N", x + 64, y, 10, nes.cpu.status & cpu6502::N ? GREEN : RED);
    DrawText("V", x + 80, y, 10, nes.cpu.status & cpu6502::V ? GREEN : RED);
    DrawText("U", x + 96, y, 10, nes.cpu.status & cpu6502::U ? GREEN : RED);
    DrawText("B", x + 112, y, 10, nes.cpu.status & cpu6502::B ? GREEN : RED);
    DrawText("D", x + 128, y, 10, nes.cpu.status & cpu6502::D ? GREEN : RED);
    DrawText("I", x + 144, y, 10, nes.cpu.status & cpu6502::I ? GREEN : RED);
    DrawText("Z", x + 160, y, 10, nes.cpu.status & cpu6502::Z ? GREEN : RED);
    DrawText("C", x + 178, y, 10, nes.cpu.status & cpu6502::C ? GREEN : RED);

    DrawString("PC: $" + hex(nes.cpu.pc, 4), x, y + 20, 10, WHITE);
    DrawString("A: $" + hex(nes.cpu.a, 2) + " X: $" + hex(nes.cpu.x, 2) + " Y: $" + hex(nes.cpu.y, 2) + " SP: $" + hex(nes.cpu.sp, 2), x, y + 40, 10, WHITE);
    DrawString("Stack P: $" + hex(nes.cpu.sp, 4), x, y + 60, 10, WHITE);
}

void emulator::draw_ram(i32 x, i32 y, u16 addr, u16 rows, u16 cols) {
    i32 ramX = x;
    i32 ramY = y;

    for(i32 row = 0; row < rows; row++) {
        std::string offset = "$" + hex(addr, 4) + ":";
        for(i32 col = 0; col < cols; col++) {
            offset += " " + hex(nes.read(addr, true), 2);
            addr++;
        }
        DrawString(offset, ramX, ramY, 10, WHITE);
        ramY += 10;
    }
}

void emulator::draw_code(i32 x, i32 y, u16 addr, u16 rows, u16 cols) {

}

