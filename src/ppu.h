//
// Created by user on 06.07.2024.
//

#ifndef PPU_H
#define PPU_H
#include <memory>

#include "cartridge.h"
#include "defines.h"
#include "raylib.h"

struct sprite {
    sprite(u32 w, u32 h) : w(w), h(h) {
        data = new Color[w * h];
    }

    ~sprite() {
        delete[] data;
    }

    void set_pixel(u32 x, u32 y, Color c) {
        if (x >= w || y >= h) return;
        data[y * w + x] = c;
    }

    u32 w, h;
    Color* data;
};

class ppu {
public:
    ppu();

    void connect_cartridge(const std::shared_ptr<cartridge>& cart);
    void clock();

    void cpu_write(u16 addr, u8 data);
    u8 cpu_read(u16 addr, bool readonly = false);

    void ppu_write(u16 addr, u8 data);
    u8 ppu_read(u16 addr, bool readonly = false);

    sprite& get_screen() { return spr_screen; }
    sprite& get_name_table(u8 i) { return spr_name_table[i]; }
    sprite& get_pattern_table(u8 i) { return spr_pattern_table[i]; }
    bool frame_complete = false;
private:
    std::shared_ptr<cartridge> cart = nullptr;

    u8 name_table[2][1024]{};
    u8 palette_table[32]{};
    u8 pattern_table[2][4096]{};

    Color pal_screen[0x40]{};
    sprite spr_screen = sprite(256, 240);
    sprite spr_name_table[2] = {sprite(256, 240), sprite(256, 240)};
    sprite spr_pattern_table[2] = {sprite(128, 128), sprite(128, 128)};

    i16 scanline{};
    i16 cycle{};
};



#endif //PPU_H
