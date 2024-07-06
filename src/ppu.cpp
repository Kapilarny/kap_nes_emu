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
	auto increment_scroll_x = [&]() {
		if(mask.render_background || mask.render_sprites) {
			if(vram_addr.coarse_x == 31) {
				vram_addr.coarse_x = 0;
				vram_addr.nametable_x = ~vram_addr.nametable_x;
			} else {
				vram_addr.coarse_x++;
			}
		}
	};

	auto increment_scroll_y = [&]() {
		if(mask.render_background || mask.render_sprites) {
			if(vram_addr.fine_y < 7) {
				vram_addr.fine_y++;
			} else {
				vram_addr.fine_y = 0;
				if(vram_addr.coarse_y == 29) {
					vram_addr.coarse_y = 0;
					vram_addr.nametable_y = ~vram_addr.nametable_y;
				} else if(vram_addr.coarse_y == 31) {
					vram_addr.coarse_y = 0;
				} else {
					vram_addr.coarse_y++;
				}
			}
		}
	};

	auto transfer_address_x = [&]() {
		if(mask.render_background || mask.render_sprites) {
			vram_addr.nametable_x = tram_addr.nametable_x;
			vram_addr.coarse_x = tram_addr.coarse_x;
		}
	};

	auto transfer_address_y = [&]() {
		if(mask.render_background || mask.render_sprites) {
			vram_addr.fine_y = tram_addr.fine_y;
			vram_addr.nametable_y = tram_addr.nametable_y;
			vram_addr.coarse_y = tram_addr.coarse_y;
		}
	};

	auto load_bg_shifters = [&]() {
		bg_shifter_pattern_lo = (bg_shifter_pattern_lo & 0xFF00) | bg_next_tile_lsb;
		bg_shifter_pattern_hi = (bg_shifter_pattern_hi & 0xFF00) | bg_next_tile_msb;

		bg_shifter_attrib_lo = (bg_shifter_attrib_lo & 0xFF00) | ((bg_next_tile_attrib & 0b01) ? 0xFF : 0x00);
		bg_shifter_attrib_hi = (bg_shifter_attrib_hi & 0xFF00) | ((bg_next_tile_attrib & 0b10) ? 0xFF : 0x00);
	};

	auto update_shifters = [&]() {
		if(mask.render_background) {
			bg_shifter_pattern_lo <<= 1;
			bg_shifter_pattern_hi <<= 1;

			bg_shifter_attrib_lo <<= 1;
			bg_shifter_attrib_hi <<= 1;
		}
	};

	if(scanline >= -1 && scanline < 240) {
		if(scanline == -1 && cycle == 1) {
			status.vertical_blank = 0;
		}

		if((cycle >= 2 && cycle < 258) || (cycle >= 321 && cycle < 338)) {
			update_shifters();

			switch ((cycle - 1) % 8) {
				case 0:
					load_bg_shifters();
					bg_next_tile_id = ppu_read(0x2000 | (vram_addr.reg & 0x0FFF));
					break;
				case 2:
					bg_next_tile_attrib = ppu_read(0x23C0 | (vram_addr.nametable_y << 11) | (vram_addr.nametable_x << 10) | ((vram_addr.coarse_y >> 2) << 3) | (vram_addr.coarse_x >> 2));
					if(vram_addr.coarse_y & 0x02) bg_next_tile_attrib >>= 4;
					if(vram_addr.coarse_x & 0x02) bg_next_tile_attrib >>= 2;
					bg_next_tile_attrib &= 0x03;
					break;
				case 4:
					bg_next_tile_lsb = ppu_read((control.pattern_background << 12) + ((u16)bg_next_tile_id << 4) + vram_addr.fine_y);
					break;
				case 6:
					bg_next_tile_msb = ppu_read((control.pattern_background << 12) + ((u16)bg_next_tile_id << 4) + vram_addr.fine_y + 8);
					break;
				case 7:
					increment_scroll_x();
					break;
				default: break;
			}
		}

		if(cycle == 256) {
			increment_scroll_y();
		}

		if(cycle == 257) {
			transfer_address_x();
		}

		if(scanline == -1 && cycle >= 280 && cycle < 305) {
			transfer_address_y();
		}
	}

	if(scanline == 240) {
		// Do nothing
	}

	if(scanline == 241 && cycle == 1) {
		status.vertical_blank = 1;
		if(control.enable_nmi) nmi = true;
	}

    // Noise (temporary)
    // spr_screen.set_pixel(cycle - 1, scanline, pal_screen[(rand() % 2) ? 0x3F : 0x30]);

	u8 bg_pixel = 0x00;
	u8 bg_palette = 0x00;

	if(mask.render_background) {
		u16 bit_mux = 0x8000 >> fine_x;

		u8 p0_pixel = (bg_shifter_pattern_lo & bit_mux) > 0;
		u8 p1_pixel = (bg_shifter_pattern_hi & bit_mux) > 0;

		bg_pixel = (p1_pixel << 1) | p0_pixel;

		u8 bg_pal0 = (bg_shifter_attrib_lo & bit_mux) > 0;
		u8 bg_pal1 = (bg_shifter_attrib_hi & bit_mux) > 0;

		bg_palette = (bg_pal1 << 1) | bg_pal0;
	}

	spr_screen.set_pixel(cycle - 1, scanline, get_color_from_palette_ram(bg_palette, bg_pixel));

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
    		tram_addr.nametable_x = control.nametable_x;
    		tram_addr.nametable_y = control.nametable_y;
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
        	if(address_latch == 0) {
        		fine_x = data & 0x07;
        		tram_addr.coarse_x = data >> 3;
        		address_latch = 1;
        	} else {
        		tram_addr.fine_y = data & 0x07;
        		tram_addr.coarse_y = data >> 3;
        		address_latch = 0;
        	}
            break;
        case 0x0006: // PPU Address
        	if(address_latch == 0) {
        		tram_addr.reg = (tram_addr.reg & 0x00FF) | (data << 8);
        		address_latch = 1;
        	} else {
        		tram_addr.reg = (tram_addr.reg & 0xFF00) | data;
        		vram_addr = tram_addr;
        		address_latch = 0;
			}
            break;
        case 0x0007: // PPU Data
        	ppu_write(vram_addr.reg, data);
    		vram_addr.reg += control.increment_mode ? 32 : 1;
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
    		ppu_data_buffer = ppu_read(vram_addr.reg);

    		if(vram_addr.reg >= 0x3F00) data = ppu_data_buffer;
    		vram_addr.reg++;
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
		addr &= 0x0FFF;

		if(cart->mirror == cartridge::MIRROR::VERTICAL) {
			if(addr >= 0x0000 && addr <= 0x03FF) {
				name_table[0][addr & 0x03FF] = data;
			} else if(addr >= 0x0400 && addr <= 0x07FF) {
				name_table[1][addr & 0x03FF] = data;
			} else if(addr >= 0x0800 && addr <= 0x0BFF) {
				name_table[0][addr & 0x03FF] = data;
			} else if(addr >= 0x0C00 && addr <= 0x0FFF) {
				name_table[1][addr & 0x03FF] = data;
			}
		} else if(cart->mirror == cartridge::MIRROR::HORIZONTAL) {
			if(addr >= 0x0000 && addr <= 0x03FF) {
				name_table[0][addr & 0x03FF] = data;
			} else if(addr >= 0x0400 && addr <= 0x07FF) {
				name_table[0][addr & 0x03FF] = data;
			} else if(addr >= 0x0800 && addr <= 0x0BFF) {
				name_table[1][addr & 0x03FF] = data;
			} else if(addr >= 0x0C00 && addr <= 0x0FFF) {
				name_table[1][addr & 0x03FF] = data;
			}
		}
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
		addr &= 0x0FFF;

		if(cart->mirror == cartridge::MIRROR::VERTICAL) {
			if(addr >= 0x0000 && addr <= 0x03FF) {
				data = name_table[0][addr & 0x03FF];
			} else if(addr >= 0x0400 && addr <= 0x07FF) {
				data = name_table[1][addr & 0x03FF];
			} else if(addr >= 0x0800 && addr <= 0x0BFF) {
				data = name_table[0][addr & 0x03FF];
			} else if(addr >= 0x0C00 && addr <= 0x0FFF) {
				data = name_table[1][addr & 0x03FF];
			}
		} else if(cart->mirror == cartridge::MIRROR::HORIZONTAL) {
			if(addr >= 0x0000 && addr <= 0x03FF) {
				data = name_table[0][addr & 0x03FF];
			} else if(addr >= 0x0400 && addr <= 0x07FF) {
				data = name_table[0][addr & 0x03FF];
			} else if(addr >= 0x0800 && addr <= 0x0BFF) {
				data = name_table[1][addr & 0x03FF];
			} else if(addr >= 0x0C00 && addr <= 0x0FFF) {
				data = name_table[1][addr & 0x03FF];
			}
		}
	} else if(addr >= 0x3F00 && addr <= 0x3FFF) {
		addr &= 0x001F;
		if(addr == 0x0010) addr = 0x0000;
		if(addr == 0x0014) addr = 0x0004;
		if(addr == 0x0018) addr = 0x0008;
		if(addr == 0x001C) addr = 0x000C;
		data = palette_table[addr];
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

	// if(ret.r != 'T' && ret.g != 'T' && ret.b != 'T') {
	// 	// LOG("Different color!!!"); // I tried to BP on this line lmao
	// }

	return ret;
	// return pal_screen[ppu_read(0x3F00 + (palette << 2) + pixel) & 0x3F];
}

