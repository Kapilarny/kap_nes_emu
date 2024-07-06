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
    cart = std::make_shared<cartridge>("nestest.nes");

    // Insert the cartridge into the NES
    nes.insert_cartridge(cart);

    // Extract the map of the assembled code
    map_asm = nes.cpu.disassemble(0x0000, 0xFFFF);

    nes.reset();
}

void emulator::update() {
    if(emulation_run) {
        do { nes.clock(); } while (!nes.ppu.frame_complete);
        nes.ppu.frame_complete = false;
    } else {
        if(IsKeyPressed(KEY_C)) {
            do { nes.clock(); } while (!nes.cpu.complete());

            do { nes.clock(); } while (nes.cpu.complete());
        }

        if(IsKeyPressed(KEY_F)) {
            do { nes.clock(); } while (!nes.ppu.frame_complete);

            nes.ppu.frame_complete = false;
        }
    }

    if(IsKeyPressed(KEY_R)) nes.reset();
    if(IsKeyPressed(KEY_SPACE)) emulation_run = !emulation_run;
    if(IsKeyPressed(KEY_P)) {
        (++selected_palette) &= 0x07;
        LOG("Selected Palette: %d", selected_palette);
    }
}

void emulator::draw() {
    // draw_ram(2 * 2, 2 * 2, 0x0000, 16, 16);
    // draw_ram(2 * 2, 182 * 2, 0x8000, 16, 16);
    // draw_cpu(448 * 2, 2 * 2);
    // draw_code(448 * 2, 72 * 2, 26);

    // DrawString("SPACE = Step Instruction | R = RESET | I = IRQ | N = NMI", 10 * 2, 370 * 2, 10 * 2, WHITE);

    draw_cpu(516 * 2, 2 * 2);
    draw_code(516 * 2, 72 * 2, 26);

    // Draw the pattern tables
    const i32 swatch_size = 6;
    for(i32 p = 0; p < 8; p++) {
        for(i32 s = 0; s < 4; s++) {
            DrawRectangle(516 * 2 + p * (swatch_size * 5) + s * swatch_size, 340 * 2, swatch_size, swatch_size, nes.ppu.get_color_from_palette_ram(p, s));
        }
    }

    draw_sprite(516 * 2, 370 * 2, nes.ppu.get_pattern_table(0, selected_palette), 2);
    draw_sprite(648 * 2, 370 * 2, nes.ppu.get_pattern_table(1, selected_palette), 2);

    draw_sprite(0, 0, nes.ppu.get_screen(), 4);
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
            offset += " " + hex(nes.cpu_read(addr, true), 2);
            addr++;
        }
        DrawString(offset, ramX, ramY, 10 * 2, WHITE);
        ramY += 10 * 2;
    }
}

void emulator::draw_code(i32 x, i32 y, i32 lines) {
    auto it_a = map_asm.find(nes.cpu.pc);
    i32 lineY = (lines >> 1) * 10 * 2 + y;
    if(it_a != map_asm.end()) {
        DrawString((*it_a).second, x, lineY, 10 * 2, SKYBLUE);
        while(lineY < (lines * 10 * 2) + y) {
            lineY += 10 * 2;
            if(++it_a != map_asm.begin()) {
                DrawString((*it_a).second, x, lineY, 10 * 2, WHITE);
            }
        }
    }

    it_a = map_asm.find(nes.cpu.pc);
    lineY = (lines >> 1) * 10 * 2 + y;
    if(it_a != map_asm.begin()) {
        while(lineY > y) {
            lineY -= 10 * 2;
            if(--it_a != map_asm.begin()) {
                DrawString((*it_a).second, x, lineY, 10 * 2, WHITE);
            }
        }
    }
}

void emulator::draw_sprite(i32 x, i32 y, const sprite &spr, i32 scale) {
    for(i32 j = 0; j < spr.h; j++) {
        for(i32 i = 0; i < spr.w; i++) {
            Color c = spr.get_pixel(i, j);
            DrawRectangle(x + i * scale, y + j * scale, scale, scale, c);
        }
    }
}
