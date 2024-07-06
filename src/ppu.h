//
// Created by user on 06.07.2024.
//

#ifndef PPU_H
#define PPU_H
#include <memory>

#include "cartridge.h"
#include "defines.h"
#include "logger.h"
#include "raylib.h"

struct sprite {
    sprite(u32 w, u32 h) : w(w), h(h) {
        // 2 dimensional array
        data = new Color*[w];
        for (u32 i = 0; i < w; i++) {
            data[i] = new Color[h];
        }

        // Fill with BLACK
        for (u32 x = 0; x < w; x++) {
            for (u32 y = 0; y < h; y++) {
                data[x][y] = BLACK;
            }
        }
    }

    ~sprite() {
        delete[] data;
    }

    void set_pixel(u32 x, u32 y, Color c) {
        if (x >= w || y >= h) return;
        // LOG("Setting pixel at x: %d, y: %d, color: %d, %d, %d, %d", x, y, c, c.r, c.g, c.b);
        data[x][y] = c;
    }

    Color get_pixel(u32 x, u32 y) const {
        if (x >= w || y >= h) return BLANK;
        return data[x][y];
    }

    u32 w, h;
    Color** data;
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

    Color get_color_from_palette_ram(u8 palette, u8 pixel);

    sprite& get_screen() { return spr_screen; }
    sprite& get_name_table(u8 i);
    sprite& get_pattern_table(u8 i, u8 palette);
    bool frame_complete = false;

    Color pal_screen[0x40]{};
    u8 name_table[2][1024]{};
    u8 palette_table[32]{};
    u8 pattern_table[2][4096]{};
    bool nmi = false;
private:
    std::shared_ptr<cartridge> cart = nullptr;

    sprite spr_screen = sprite(256, 240);
    sprite spr_name_table[2] = {sprite(256, 240), sprite(256, 240)};
    sprite spr_pattern_table[2] = {sprite(128, 128), sprite(128, 128)};

    i16 scanline{};
    i16 cycle{};

    union {
        struct {
            u8 unused : 5;
            u8 sprite_overflow : 1;
            u8 sprite_zero_hit : 1;
            u8 vertical_blank : 1;
        };

        u8 reg;
    } status;

    union {
        struct {
            u8 grayscale : 1;
            u8 render_background_left : 1;
            u8 render_sprites_left : 1;
            u8 render_background : 1;
            u8 render_sprites : 1;
            u8 enhance_red : 1;
            u8 enhance_green : 1;
            u8 enhance_blue : 1;
        };

        u8 reg;
    } mask;

    union {
        struct {
            u8 nametable_x : 1;
            u8 nametable_y : 1;
            u8 increment_mode : 1;
            u8 pattern_sprite : 1;
            u8 pattern_background : 1;
            u8 sprite_size : 1;
            u8 slave_mode : 1; // unused
            u8 enable_nmi : 1;
        };

        u8 reg;
    } control;

    u8 address_latch{};
    u8 ppu_data_buffer{};
    // u16 ppu_address{};

    union loopy_register {
        u16 reg = 0x0000;
        struct {
            u16 coarse_x : 5;
            u16 coarse_y : 5;
            u16 nametable_x : 1;
            u16 nametable_y : 1;
            u16 fine_y : 3;
            u16 unused : 1;
        };
    };

    loopy_register vram_addr{};
    loopy_register tram_addr{};

    u8 fine_x{};

    u8 bg_next_tile_id{};
    u8 bg_next_tile_attrib{};
    u8 bg_next_tile_lsb{};
    u8 bg_next_tile_msb{};
};



#endif //PPU_H
