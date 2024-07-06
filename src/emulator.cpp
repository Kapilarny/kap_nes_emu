//
// Created by user on 06.07.2024.
//

#include "emulator.h"

#include <sstream>
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

emulator::emulator() {

    /*
            *=$8000
            LDX #10
            STX $0000
            LDX #3
            STX $0001
            LDY $0000
            LDA #0
            CLC
            loop
            ADC $0001
            DEY
            BNE loop
            STA $0002
            NOP
            NOP
            NOP
    */
    std::stringstream ss;
    ss << "A2 0A 8E 00 00 A2 03 8E 01 00 AC 00 00 A9 00 18 6D 01 00 88 D0 FA 8D 02 00 EA EA EA";
    u16 offset = 0x8000;

    while(!ss.eof()) {
        std::string b;
        ss >> b;
        nes.ram[offset++] = (u8) std::stoul(b, nullptr, 16);
    }

    // Set Reset Vector
    nes.ram[0xFFFC] = 0x00;
    nes.ram[0xFFFD] = 0x80;

    // Extract disasm
    mapAsm = nes.cpu.disassemble(0x0000, 0xFFFF);

    // Reset
    nes.cpu.reset();
}

void emulator::update() {
    if(IsKeyPressed(KEY_SPACE)) {
        do {
            nes.cpu.clock();
        } while(!nes.cpu.complete());
    }

    if(IsKeyPressed(KEY_R)) nes.cpu.reset();
    if(IsKeyPressed(KEY_I)) nes.cpu.irq();
    if(IsKeyPressed(KEY_N)) nes.cpu.nmi();
}

void emulator::draw() {
    draw_ram(2 * 2, 2 * 2, 0x0000, 16, 16);
    draw_ram(2 * 2, 182 * 2, 0x8000, 16, 16);
    draw_cpu(448 * 2, 2 * 2);
    draw_code(448 * 2, 72 * 2, 26);

    DrawString("SPACE = Step Instruction | R = RESET | I = IRQ | N = NMI", 10 * 2, 370 * 2, 10 * 2, WHITE);
}

void emulator::draw_cpu(i32 x, i32 y) {
    std::string status = "STATUS: ";
    DrawText(status.c_str(), x, y, 10 * 2, WHITE);
    DrawText("N", x + 64 + 32, y, 10 * 2, nes.cpu.status & cpu6502::N ? GREEN : RED);
    DrawText("V", x + 80 + 32, y, 10 * 2, nes.cpu.status & cpu6502::V ? GREEN : RED);
    DrawText("U", x + 96 + 32, y, 10 * 2, nes.cpu.status & cpu6502::U ? GREEN : RED);
    DrawText("B", x + 112 + 32, y, 10 * 2, nes.cpu.status & cpu6502::B ? GREEN : RED);
    DrawText("D", x + 128 + 32, y, 10 * 2, nes.cpu.status & cpu6502::D ? GREEN : RED);
    DrawText("I", x + 144 + 32, y, 10 * 2, nes.cpu.status & cpu6502::I ? GREEN : RED);
    DrawText("Z", x + 160 + 32, y, 10 * 2, nes.cpu.status & cpu6502::Z ? GREEN : RED);
    DrawText("C", x + 178 + 32, y, 10 * 2, nes.cpu.status & cpu6502::C ? GREEN : RED);

    DrawString("PC: $" + hex(nes.cpu.pc, 4), x, y + 20, 10 * 2, WHITE);
    DrawString("A: $" + hex(nes.cpu.a, 2) + " [" + std::to_string(nes.cpu.a) + "]", x, y + 40, 10 * 2, WHITE);
    DrawString("X: $" + hex(nes.cpu.x, 2) + " [" + std::to_string(nes.cpu.x) + "]", x, y + 60, 10 * 2, WHITE);
    DrawString("Y: $" + hex(nes.cpu.y, 2) + " [" + std::to_string(nes.cpu.y) + "]", x, y + 80, 10 * 2, WHITE);
    DrawString("Stack P: $" + hex(nes.cpu.sp, 4), x, y + 100, 10 * 2, WHITE);
    // DrawString("Stack P: $" + hex(nes.cpu.sp, 4), x, y + 60, 10 * 2, WHITE);
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
        DrawString(offset, ramX, ramY, 10 * 2, WHITE);
        ramY += 10 * 2;
    }
}

void emulator::draw_code(i32 x, i32 y, i32 lines) {
    auto it_a = mapAsm.find(nes.cpu.pc);
    i32 lineY = (lines >> 1) * 10 * 2 + y;
    if(it_a != mapAsm.end()) {
        DrawString((*it_a).second, x, lineY, 10 * 2, SKYBLUE);
        while(lineY < (lines * 10 * 2) + y) {
            lineY += 10 * 2;
            if(++it_a != mapAsm.begin()) {
                DrawString((*it_a).second, x, lineY, 10 * 2, WHITE);
            }
        }
    }

    it_a = mapAsm.find(nes.cpu.pc);
    lineY = (lines >> 1) * 10 * 2 + y;
    if(it_a != mapAsm.begin()) {
        while(lineY > y) {
            lineY -= 10 * 2;
            if(--it_a != mapAsm.begin()) {
                DrawString((*it_a).second, x, lineY, 10 * 2, WHITE);
            }
        }
    }
}
