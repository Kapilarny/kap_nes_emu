//
// Created by user on 06.07.2024.
//

#include "ppu.h"

ppu::ppu() {
    pal_screen[0x00] = Color{84, 84, 84};
	pal_screen[0x01] = Color{0, 30, 116};
	pal_screen[0x02] = Color{8, 16, 144};
	pal_screen[0x03] = Color{48, 0, 136};
	pal_screen[0x04] = Color{68, 0, 100};
	pal_screen[0x05] = Color{92, 0, 48};
	pal_screen[0x06] = Color{84, 4, 0};
	pal_screen[0x07] = Color{60, 24, 0};
	pal_screen[0x08] = Color{32, 42, 0};
	pal_screen[0x09] = Color{8, 58, 0};
	pal_screen[0x0A] = Color{0, 64, 0};
	pal_screen[0x0B] = Color{0, 60, 0};
	pal_screen[0x0C] = Color{0, 50, 60};
	pal_screen[0x0D] = Color{0, 0, 0};
	pal_screen[0x0E] = Color{0, 0, 0};
	pal_screen[0x0F] = Color{0, 0, 0};

	pal_screen[0x10] = Color{152, 150, 152};
	pal_screen[0x11] = Color{8, 76, 196};
	pal_screen[0x12] = Color{48, 50, 236};
	pal_screen[0x13] = Color{92, 30, 228};
	pal_screen[0x14] = Color{136, 20, 176};
	pal_screen[0x15] = Color{160, 20, 100};
	pal_screen[0x16] = Color{152, 34, 32};
	pal_screen[0x17] = Color{120, 60, 0};
	pal_screen[0x18] = Color{84, 90, 0};
	pal_screen[0x19] = Color{40, 114, 0};
	pal_screen[0x1A] = Color{8, 124, 0};
	pal_screen[0x1B] = Color{0, 118, 40};
	pal_screen[0x1C] = Color{0, 102, 120};
	pal_screen[0x1D] = Color{0, 0, 0};
	pal_screen[0x1E] = Color{0, 0, 0};
	pal_screen[0x1F] = Color{0, 0, 0};

	pal_screen[0x20] = Color{236, 238, 236};
	pal_screen[0x21] = Color{76, 154, 236};
	pal_screen[0x22] = Color{120, 124, 236};
	pal_screen[0x23] = Color{176, 98, 236};
	pal_screen[0x24] = Color{228, 84, 236};
	pal_screen[0x25] = Color{236, 88, 180};
	pal_screen[0x26] = Color{236, 106, 100};
	pal_screen[0x27] = Color{212, 136, 32};
	pal_screen[0x28] = Color{160, 170, 0};
	pal_screen[0x29] = Color{116, 196, 0};
	pal_screen[0x2A] = Color{76, 208, 32};
	pal_screen[0x2B] = Color{56, 204, 108};
	pal_screen[0x2C] = Color{56, 180, 204};
	pal_screen[0x2D] = Color{60, 60, 60};
	pal_screen[0x2E] = Color{0, 0, 0};
	pal_screen[0x2F] = Color{0, 0, 0};

	pal_screen[0x30] = Color{236, 238, 236};
	pal_screen[0x31] = Color{168, 204, 236};
	pal_screen[0x32] = Color{188, 188, 236};
	pal_screen[0x33] = Color{212, 178, 236};
	pal_screen[0x34] = Color{236, 174, 236};
	pal_screen[0x35] = Color{236, 174, 212};
	pal_screen[0x36] = Color{236, 180, 176};
	pal_screen[0x37] = Color{228, 196, 144};
	pal_screen[0x38] = Color{204, 210, 120};
	pal_screen[0x39] = Color{180, 222, 120};
	pal_screen[0x3A] = Color{168, 226, 144};
	pal_screen[0x3B] = Color{152, 226, 180};
	pal_screen[0x3C] = Color{160, 214, 228};
	pal_screen[0x3D] = Color{160, 162, 160};
	pal_screen[0x3E] = Color{0, 0, 0};
	pal_screen[0x3F] = Color{0, 0, 0};
}

void ppu::connect_cartridge(const std::shared_ptr<cartridge>& cart) {
    this->cart = cart;
}

void ppu::clock() {
    // Noise (temporary)
    spr_screen.set_pixel(cycle - 1, scanline, pal_screen[(rand() % 2) ? 0x3F : 0x30]);

    cycle++;
    if(cycle >= 341) {
        cycle = 0;
        scanline++;
        if(scanline >= 261) {
            scanline = -1;
            frame_complete = true;
        }
    }
}

void ppu::cpu_write(u16 addr, u8 data) {
    switch (addr) {
        case 0x0000: // Control
            break;
        case 0x0001: // Mask
            break;
        case 0x0002: // Status
            break;
        case 0x0003: // OAM Address
            break;
        case 0x0004: // OAM Data
            break;
        case 0x0005: // Scroll
            break;
        case 0x0006: // PPU Address
            break;
        case 0x0007: // PPU Data
            break;
    }
}

u8 ppu::cpu_read(u16 addr, bool readonly) {
    u8 data = 0x00;

    switch(addr) {
        case 0x0000: // Control
            break;
        case 0x0001: // Mask
            break;
        case 0x0002: // Status
            break;
        case 0x0003: // OAM Address
            break;
        case 0x0004: // OAM Data
            break;
        case 0x0005: // Scroll
            break;
        case 0x0006: // PPU Address
            break;
        case 0x0007: // PPU Data
            break;
    }

    return data;
}

void ppu::ppu_write(u16 addr, u8 data) {
    addr &= 0x3FFF;

    if(cart->ppu_write(addr, data)) {}
}

u8 ppu::ppu_read(u16 addr, bool readonly) {
    u8 data = 0x00;

    if(cart->ppu_read(addr, data)) {}

    addr &= 0x3FFF;
    return data;
}
