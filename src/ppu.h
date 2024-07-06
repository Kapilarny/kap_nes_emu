//
// Created by user on 06.07.2024.
//

#ifndef PPU_H
#define PPU_H
#include <memory>
#include <olcPixelGameEngine.h>

#include "cartridge.h"
#include "defines.h"
#include "logger.h"
// #include "raylib.h"

class ppu {
public:
    ppu();

    void connect_cartridge(const std::shared_ptr<cartridge>& cart);
    void clock();

    void cpu_write(u16 addr, u8 data);
    u8 cpu_read(u16 addr, bool readonly = false);

    void ppu_write(u16 addr, u8 data);
    u8 ppu_read(u16 addr, bool readonly = false);

    olc::Pixel get_color_from_palette_ram(u8 palette, u8 pixel);

    olc::Sprite& get_screen();
    olc::Sprite& get_name_table(u8 i);
    olc::Sprite& get_pattern_table(u8 i, u8 palette);
    bool frame_complete = false;

    olc::Pixel pal_screen[0x40]{};
    u8 name_table[2][1024]{};
    u8 palette_table[32]{};
    u8 pattern_table[2][4096]{};
    bool nmi = false;

    u8* p_oam = (u8*)oam;
    u8 oam_addr = 0x00;
private:
    std::shared_ptr<cartridge> cart = nullptr;

    olc::Sprite* spr_screen;
    olc::Sprite* spr_name_table[2];
    olc::Sprite* spr_pattern_table[2];

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

    u16 bg_shifter_pattern_lo{};
    u16 bg_shifter_pattern_hi{};
    u16 bg_shifter_attrib_lo{};
    u16 bg_shifter_attrib_hi{};

    struct object_attrib_entry {
        u8 y;
        u8 id;
        u8 attribute;
        u8 x;
    } oam[64]{};

    object_attrib_entry sprite_scanline[8]{};
    u8 sprite_count{};
    u8 sprite_shifter_pattern_lo[8]{};
    u8 sprite_shifter_pattern_hi[8]{};
};



#endif //PPU_H
