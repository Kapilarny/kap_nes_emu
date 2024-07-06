//
// Created by user on 06.07.2024.
//

#include "ppu.h"

ppu::ppu() {
    pal_screen[0x00] = Color{84, 84, 84, 255};
	pal_screen[0x01] = Color{0, 30, 116, 255};
	pal_screen[0x02] = Color{8, 16, 144, 255};
	pal_screen[0x03] = Color{48, 0, 136, 255};
	pal_screen[0x04] = Color{68, 0, 100, 255};
	pal_screen[0x05] = Color{92, 0, 48, 255};
	pal_screen[0x06] = Color{84, 4, 0, 255};
	pal_screen[0x07] = Color{60, 24, 0, 255};
	pal_screen[0x08] = Color{32, 42, 0, 255};
	pal_screen[0x09] = Color{8, 58, 0, 255};
	pal_screen[0x0A] = Color{0, 64, 0, 255};
	pal_screen[0x0B] = Color{0, 60, 0, 255};
	pal_screen[0x0C] = Color{0, 50, 60, 255};
	pal_screen[0x0D] = Color{0, 0, 0, 255};
	pal_screen[0x0E] = Color{0, 0, 0, 255};
	pal_screen[0x0F] = Color{0, 0, 0, 255};

	pal_screen[0x10] = Color{152, 150, 152, 255};
	pal_screen[0x11] = Color{8, 76, 196, 255};
	pal_screen[0x12] = Color{48, 50, 236, 255};
	pal_screen[0x13] = Color{92, 30, 228, 255};
	pal_screen[0x14] = Color{136, 20, 176, 255};
	pal_screen[0x15] = Color{160, 20, 100, 255};
	pal_screen[0x16] = Color{152, 34, 32, 255};
	pal_screen[0x17] = Color{120, 60, 0, 255};
	pal_screen[0x18] = Color{84, 90, 0, 255};
	pal_screen[0x19] = Color{40, 114, 0, 255};
	pal_screen[0x1A] = Color{8, 124, 0, 255};
	pal_screen[0x1B] = Color{0, 118, 40, 255};
	pal_screen[0x1C] = Color{0, 102, 120, 255};
	pal_screen[0x1D] = Color{0, 0, 0, 255};
	pal_screen[0x1E] = Color{0, 0, 0, 255};
	pal_screen[0x1F] = Color{0, 0, 0, 255};

	pal_screen[0x20] = Color{236, 238, 236, 255};
	pal_screen[0x21] = Color{76, 154, 236, 255};
	pal_screen[0x22] = Color{120, 124, 236, 255};
	pal_screen[0x23] = Color{176, 98, 236, 255};
	pal_screen[0x24] = Color{228, 84, 236, 255};
	pal_screen[0x25] = Color{236, 88, 180, 255};
	pal_screen[0x26] = Color{236, 106, 100, 255};
	pal_screen[0x27] = Color{212, 136, 32, 255};
	pal_screen[0x28] = Color{160, 170, 0, 255};
	pal_screen[0x29] = Color{116, 196, 0, 255};
	pal_screen[0x2A] = Color{76, 208, 32, 255};
	pal_screen[0x2B] = Color{56, 204, 108, 255};
	pal_screen[0x2C] = Color{56, 180, 204, 255};
	pal_screen[0x2D] = Color{60, 60, 60, 255};
	pal_screen[0x2E] = Color{0, 0, 0, 255};
	pal_screen[0x2F] = Color{0, 0, 0, 255};

	pal_screen[0x30] = Color{236, 238, 236, 255};
	pal_screen[0x31] = Color{168, 204, 236, 255};
	pal_screen[0x32] = Color{188, 188, 236, 255};
	pal_screen[0x33] = Color{212, 178, 236, 255};
	pal_screen[0x34] = Color{236, 174, 236, 255};
	pal_screen[0x35] = Color{236, 174, 212, 255};
	pal_screen[0x36] = Color{236, 180, 176, 255};
	pal_screen[0x37] = Color{228, 196, 144, 255};
	pal_screen[0x38] = Color{204, 210, 120, 255};
	pal_screen[0x39] = Color{180, 222, 120, 255};
	pal_screen[0x3A] = Color{168, 226, 144, 255};
	pal_screen[0x3B] = Color{152, 226, 180, 255};
	pal_screen[0x3C] = Color{160, 214, 228, 255};
	pal_screen[0x3D] = Color{160, 162, 160, 255};
	pal_screen[0x3E] = Color{0, 0, 0, 255};
	pal_screen[0x3F] = Color{0, 0, 0, 255};
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
        	control.reg = data;
            break;
        case 0x0001: // Mask
        	mask.reg = data;
            break;
        case 0x0002: // Status
        	// status.reg = data;
            break;
        case 0x0003: // OAM Address
            break;
        case 0x0004: // OAM Data
            break;
        case 0x0005: // Scroll
            break;
        case 0x0006: // PPU Address
        	if(address_latch == 0) {
        		ppu_address = (ppu_address & 0x00FF) | (data << 8);
        		address_latch = 1;
        	} else {
        		ppu_address = (ppu_address & 0xFF00) | data;
        		address_latch = 0;
			}
            break;
        case 0x0007: // PPU Data
        	ppu_write(ppu_address, data);
    		ppu_address++;
            break;
    }
}

u8 ppu::cpu_read(u16 addr, bool readonly) {
    u8 data = 0x00;

    switch(addr) {
        case 0x0000: // Control
        	data = control.reg;
            break;
        case 0x0001: // Mask
        	data = mask.reg;
            break;
        case 0x0002: // Status
        	status.vertical_blank = 1;
        	data = (status.reg & 0xE0) | (ppu_data_buffer & 0x1F);
    		status.vertical_blank = 0;
    		address_latch = 0;
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
        	data = ppu_data_buffer;
    		ppu_data_buffer = ppu_read(ppu_address);

    		if(ppu_address >= 0x3F00) data = ppu_data_buffer;
    		ppu_address++;
    		break;
    }

    return data;
}

void ppu::ppu_write(u16 addr, u8 data) {
    addr &= 0x3FFF;

    if(cart->ppu_write(addr, data)) {}
    else if(addr <= 0x1FFF) {
    	pattern_table[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;
    } else if(addr >= 0x2000 && addr <= 0x3EFF) {

    } else if(addr >= 0x3F00 && addr <= 0x3FFF) {
    	addr &= 0x001F;
    	if(addr == 0x0010) addr = 0x0000;
    	if(addr == 0x0014) addr = 0x0004;
    	if(addr == 0x0018) addr = 0x0008;
    	if(addr == 0x001C) addr = 0x000C;
    	palette_table[addr] = data;
    }
}

u8 ppu::ppu_read(u16 addr, bool readonly) {
    u8 data = 0x00;
	addr &= 0x3FFF;

    if(cart->ppu_read(addr, data)) {}
	else if(addr >= 0x0000 && addr <= 0x1FFF) {
		data = pattern_table[(addr & 0x1000) >> 12][addr & 0x0FFF];
	} else if(addr >= 0x2000 && addr <= 0x3EFF) {

	} else if(addr >= 0x3F00 && addr <= 0x3FFF) {
		addr &= 0x001F;
		if(addr == 0x0010) addr = 0x0000;
		if(addr == 0x0014) addr = 0x0004;
		if(addr == 0x0018) addr = 0x0008;
		if(addr == 0x001C) addr = 0x000C;
		data = palette_table[addr] & (mask.grayscale ? 0x30 : 0x3F);
	}

    return data;
}

sprite& ppu::get_name_table(u8 i) {
	return spr_name_table[i];
}

sprite& ppu::get_pattern_table(u8 i, u8 palette) {
	for(u16 tileY = 0; tileY < 16; tileY++) {
		for(u16 tileX = 0; tileX < 16; tileX++) {
			u16 offset = tileY * 256 + tileX * 16;

			for(u16 row = 0; row < 8; row++) {
				u8 tile_lsb = ppu_read(i * 0x1000 + offset + row + 0x0000);
				u8 tile_msb = ppu_read(i * 0x1000 + offset + row + 0x0008);

				for(u16 col = 0; col < 8; col++) {
					u8 pixel = (tile_lsb & 0x01) + (tile_msb & 0x01);
					tile_lsb >>= 1;
					tile_msb >>= 1;

					spr_pattern_table[i].set_pixel(tileX * 8 + (7 - col), tileY * 8 + row, get_color_from_palette_ram(palette, pixel));
				}
			}
		}
	}

	return spr_pattern_table[i];
}

Color ppu::get_color_from_palette_ram(u8 palette, u8 pixel) {
	auto ret = pal_screen[ppu_read(0x3F00 + (palette << 2) + pixel) & 0x3F];

	if(ret.r != 'T' && ret.g != 'T' && ret.b != 'T') {
		LOG("Different color!!!"); // I tried to BP on this line lmao
	}

	return ret;
	// return pal_screen[ppu_read(0x3F00 + (palette << 2) + pixel) & 0x3F];
}

