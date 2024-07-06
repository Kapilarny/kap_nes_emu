//
// Created by user on 06.07.2024.
//

#include "ppu.h"

ppu::ppu() {
    pal_screen[0x00] = olc::Pixel(84, 84, 84);
	pal_screen[0x01] = olc::Pixel(0, 30, 116);
	pal_screen[0x02] = olc::Pixel(8, 16, 144);
	pal_screen[0x03] = olc::Pixel(48, 0, 136);
	pal_screen[0x04] = olc::Pixel(68, 0, 100);
	pal_screen[0x05] = olc::Pixel(92, 0, 48);
	pal_screen[0x06] = olc::Pixel(84, 4, 0);
	pal_screen[0x07] = olc::Pixel(60, 24, 0);
	pal_screen[0x08] = olc::Pixel(32, 42, 0);
	pal_screen[0x09] = olc::Pixel(8, 58, 0);
	pal_screen[0x0A] = olc::Pixel(0, 64, 0);
	pal_screen[0x0B] = olc::Pixel(0, 60, 0);
	pal_screen[0x0C] = olc::Pixel(0, 50, 60);
	pal_screen[0x0D] = olc::Pixel(0, 0, 0);
	pal_screen[0x0E] = olc::Pixel(0, 0, 0);
	pal_screen[0x0F] = olc::Pixel(0, 0, 0);

	pal_screen[0x10] = olc::Pixel(152, 150, 152);
	pal_screen[0x11] = olc::Pixel(8, 76, 196);
	pal_screen[0x12] = olc::Pixel(48, 50, 236);
	pal_screen[0x13] = olc::Pixel(92, 30, 228);
	pal_screen[0x14] = olc::Pixel(136, 20, 176);
	pal_screen[0x15] = olc::Pixel(160, 20, 100);
	pal_screen[0x16] = olc::Pixel(152, 34, 32);
	pal_screen[0x17] = olc::Pixel(120, 60, 0);
	pal_screen[0x18] = olc::Pixel(84, 90, 0);
	pal_screen[0x19] = olc::Pixel(40, 114, 0);
	pal_screen[0x1A] = olc::Pixel(8, 124, 0);
	pal_screen[0x1B] = olc::Pixel(0, 118, 40);
	pal_screen[0x1C] = olc::Pixel(0, 102, 120);
	pal_screen[0x1D] = olc::Pixel(0, 0, 0);
	pal_screen[0x1E] = olc::Pixel(0, 0, 0);
	pal_screen[0x1F] = olc::Pixel(0, 0, 0);

	pal_screen[0x20] = olc::Pixel(236, 238, 236);
	pal_screen[0x21] = olc::Pixel(76, 154, 236);
	pal_screen[0x22] = olc::Pixel(120, 124, 236);
	pal_screen[0x23] = olc::Pixel(176, 98, 236);
	pal_screen[0x24] = olc::Pixel(228, 84, 236);
	pal_screen[0x25] = olc::Pixel(236, 88, 180);
	pal_screen[0x26] = olc::Pixel(236, 106, 100);
	pal_screen[0x27] = olc::Pixel(212, 136, 32);
	pal_screen[0x28] = olc::Pixel(160, 170, 0);
	pal_screen[0x29] = olc::Pixel(116, 196, 0);
	pal_screen[0x2A] = olc::Pixel(76, 208, 32);
	pal_screen[0x2B] = olc::Pixel(56, 204, 108);
	pal_screen[0x2C] = olc::Pixel(56, 180, 204);
	pal_screen[0x2D] = olc::Pixel(60, 60, 60);
	pal_screen[0x2E] = olc::Pixel(0, 0, 0);
	pal_screen[0x2F] = olc::Pixel(0, 0, 0);

	pal_screen[0x30] = olc::Pixel(236, 238, 236);
	pal_screen[0x31] = olc::Pixel(168, 204, 236);
	pal_screen[0x32] = olc::Pixel(188, 188, 236);
	pal_screen[0x33] = olc::Pixel(212, 178, 236);
	pal_screen[0x34] = olc::Pixel(236, 174, 236);
	pal_screen[0x35] = olc::Pixel(236, 174, 212);
	pal_screen[0x36] = olc::Pixel(236, 180, 176);
	pal_screen[0x37] = olc::Pixel(228, 196, 144);
	pal_screen[0x38] = olc::Pixel(204, 210, 120);
	pal_screen[0x39] = olc::Pixel(180, 222, 120);
	pal_screen[0x3A] = olc::Pixel(168, 226, 144);
	pal_screen[0x3B] = olc::Pixel(152, 226, 180);
	pal_screen[0x3C] = olc::Pixel(160, 214, 228);
	pal_screen[0x3D] = olc::Pixel(160, 162, 160);
	pal_screen[0x3E] = olc::Pixel(0, 0, 0);
	pal_screen[0x3F] = olc::Pixel(0, 0, 0);

	spr_screen = new olc::Sprite(256, 240);
	spr_name_table[0] = new olc::Sprite(256, 240);
	spr_name_table[1] = new olc::Sprite(256, 240);
	spr_pattern_table[0] = new olc::Sprite(128, 128);
	spr_pattern_table[1] = new olc::Sprite(128, 128);
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

		if(mask.render_sprites && cycle >= 1 && cycle < 258) {
			for(u8 i = 0; i < sprite_count; i++) {
				if(sprite_scanline[i].x > 0) {
					sprite_scanline[i].x--;
				} else {
					sprite_shifter_pattern_lo[i] <<= 1;
					sprite_shifter_pattern_hi[i] <<= 1;
				}
			}
		}
	};

	if(scanline >= -1 && scanline < 240) {
		if(scanline == -1 && cycle == 1) {
			status.vertical_blank = 0;

			status.sprite_overflow = 0;

			for(u8 i = 0; i < 8; i++) {
				sprite_shifter_pattern_lo[i] = 0;
				sprite_shifter_pattern_hi[i] = 0;
			}
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

		// Foreground Rendering
		if(cycle == 257 && scanline >= 0) {
			memset(sprite_scanline, 0xFF, sizeof(object_attrib_entry) * 8);
			sprite_count = 0;

			for(u8 i = 0; i < 8; i++) {
				sprite_shifter_pattern_lo[i] = 0;
				sprite_shifter_pattern_hi[i] = 0;
			}

			u8 nOAMEntry = 0;
			while(nOAMEntry < 64 && sprite_count < 9) {
				i16 diff = (i16)scanline - (i16)oam[nOAMEntry].y;
				if(diff >= 0 && diff < (control.sprite_size ? 16 : 8)) {
					if(sprite_count < 8) {
						memcpy(&sprite_scanline[sprite_count], &p_oam[nOAMEntry], sizeof(object_attrib_entry));
						sprite_count++;
					}
				}

				nOAMEntry++;
			}

			status.sprite_overflow = sprite_count > 8;
		}

		if(cycle == 340) {
			for(u8 i = 0; i < sprite_count; i++) {
				u8 sprite_pattern_bits_lo{}, sprite_pattern_bits_hi{};
				u16 sprite_pattern_addr_lo{}, sprite_pattern_addr_hi{};

				if (!control.sprite_size) {
				    // 8x8 Sprite Mode - The control register determines the pattern table
				    if (!(sprite_scanline[i].attribute & 0x80)) {
				        // Sprite is NOT flipped vertically, i.e., normal
				        sprite_pattern_addr_lo =
				          (control.pattern_sprite << 12)  // Which Pattern Table? 0KB or 4KB offset
				        | (sprite_scanline[i].id << 4)    // Which Cell? Tile ID * 16 (16 bytes per tile)
				        | (scanline - sprite_scanline[i].y); // Which Row in cell? (0->7)
				    } else {
				        // Sprite is flipped vertically, i.e., upside down
				        sprite_pattern_addr_lo =
				          (control.pattern_sprite << 12)  // Which Pattern Table? 0KB or 4KB offset
				        | (sprite_scanline[i].id << 4)    // Which Cell? Tile ID * 16 (16 bytes per tile)
				        | (7 - (scanline - sprite_scanline[i].y)); // Which Row in cell? (7->0)
				    }
				} else {
				    // 8x16 Sprite Mode - The sprite attribute determines the pattern table
				    if (!(sprite_scanline[i].attribute & 0x80)) {
				        // Sprite is NOT flipped vertically, i.e., normal
				        if (scanline - sprite_scanline[i].y < 8) {
				            // Reading Top half Tile
				            sprite_pattern_addr_lo =
				              ((sprite_scanline[i].id & 0x01) << 12)  // Which Pattern Table? 0KB or 4KB offset
				            | ((sprite_scanline[i].id & 0xFE) << 4)   // Which Cell? Tile ID * 16 (16 bytes per tile)
				            | ((scanline - sprite_scanline[i].y) & 0x07); // Which Row in cell? (0->7)
				        } else {
				            // Reading Bottom Half Tile
				            sprite_pattern_addr_lo =
				              ((sprite_scanline[i].id & 0x01) << 12)  // Which Pattern Table? 0KB or 4KB offset
				            | (((sprite_scanline[i].id & 0xFE) + 1) << 4)  // Which Cell? Tile ID * 16 (16 bytes per tile)
				            | ((scanline - sprite_scanline[i].y) & 0x07); // Which Row in cell? (0->7)
				        }
				    } else {
				        // Sprite is flipped vertically, i.e., upside down
				        if (scanline - sprite_scanline[i].y < 8) {
				            // Reading Top half Tile
				            sprite_pattern_addr_lo =
				              ((sprite_scanline[i].id & 0x01) << 12)    // Which Pattern Table? 0KB or 4KB offset
				            | (((sprite_scanline[i].id & 0xFE) + 1) << 4)    // Which Cell? Tile ID * 16 (16 bytes per tile)
				            | (7 - (scanline - sprite_scanline[i].y) & 0x07); // Which Row in cell? (0->7)
				        } else {
				            // Reading Bottom Half Tile
				            sprite_pattern_addr_lo =
				              ((sprite_scanline[i].id & 0x01) << 12)    // Which Pattern Table? 0KB or 4KB offset
				            | ((sprite_scanline[i].id & 0xFE) << 4)    // Which Cell? Tile ID * 16 (16 bytes per tile)
				            | (7 - (scanline - sprite_scanline[i].y) & 0x07); // Which Row in cell? (0->7)
				        }
				    }
				}

				sprite_pattern_addr_hi = sprite_pattern_addr_lo + 8;
				sprite_pattern_bits_lo = ppu_read(sprite_pattern_addr_lo);
				sprite_pattern_bits_hi = ppu_read(sprite_pattern_addr_hi);

				if(sprite_scanline[i].attribute & 0x40) {
					auto flipbyte = [](u8 b) {
						b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
						b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
						b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
						return b;
					};

					sprite_pattern_bits_lo = flipbyte(sprite_pattern_bits_lo);
					sprite_pattern_bits_hi = flipbyte(sprite_pattern_bits_hi);
				}

				sprite_shifter_pattern_lo[i] = sprite_pattern_bits_lo;
				sprite_shifter_pattern_hi[i] = sprite_pattern_bits_hi;
			}
		}
	}

	if(scanline == 240) {
		// Do nothing
	}

	if(scanline == 241 && cycle == 1) {
		status.vertical_blank = 1;
		if(control.enable_nmi) nmi = true;
	}

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

	u8 fg_pixel = 0x00;
	u8 fg_palette = 0x00;
	u8 fg_priority = 0x00;

	if(mask.render_sprites) {
		for(u8 i = 0; i < sprite_count; i++) {
			if(sprite_scanline[i].x == 0) {
				u8 fg_pixel_lo = (sprite_shifter_pattern_lo[i] & 0x80) > 0;
				u8 fg_pixel_hi = (sprite_shifter_pattern_hi[i] & 0x80) > 0;

				fg_pixel = (fg_pixel_hi << 1) | fg_pixel_lo;

				fg_palette = (sprite_scanline[i].attribute & 0x03) + 0x04;
				fg_priority = (sprite_scanline[i].attribute & 0x20) == 0;

				if(fg_pixel != 0) {
					// if(i == 0) status.sprite_zero_hit = 1;
					break;
				}
			}
		}
	}

	u8 pixel = 0x00;
	u8 palette = 0x00;

	if(bg_pixel == 0 && fg_pixel == 0) {
		pixel = 0x00;
		palette = 0x00;
	} else if(bg_pixel == 0 && fg_pixel > 0) {
		pixel = fg_pixel;
		palette = fg_palette;
	} else if(bg_pixel > 0 && fg_pixel == 0) {
		pixel = bg_pixel;
		palette = bg_palette;
	} else if(bg_pixel > 0 && fg_pixel > 0) {
		if(fg_priority) {
			pixel = fg_pixel;
			palette = fg_palette;
		} else {
			pixel = bg_pixel;
			palette = bg_palette;
		}
	}

	spr_screen->SetPixel(cycle - 1, scanline, get_color_from_palette_ram(palette, pixel));

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
        	oam_addr = data;
            break;
        case 0x0004: // OAM Data
        	p_oam[oam_addr] = data;
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
        	data = p_oam[oam_addr];
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

olc::Sprite& ppu::get_name_table(u8 i) {
	return *spr_name_table[i];
}

olc::Sprite& ppu::get_pattern_table(u8 i, u8 palette) {
	for(u16 tileY = 0; tileY < 16; tileY++) {
		for(u16 tileX = 0; tileX < 16; tileX++) {
			u16 offset = tileY * 256 + tileX * 16;

			for(u16 row = 0; row < 8; row++) {
				u8 tile_lsb = ppu_read(i * 0x1000 + offset + row + 0x0000);
				u8 tile_msb = ppu_read(i * 0x1000 + offset + row + 0x0008);

				for(u16 col = 0; col < 8; col++) {
					u8 pixel = ((tile_lsb & 0x01) << 1) | (tile_msb & 0x01);
					tile_lsb >>= 1;
					tile_msb >>= 1;

					spr_pattern_table[i]->SetPixel(tileX * 8 + (7 - col), tileY * 8 + row, get_color_from_palette_ram(palette, pixel));
				}
			}
		}
	}

	return *spr_pattern_table[i];
}

olc::Pixel ppu::get_color_from_palette_ram(u8 palette, u8 pixel) {
	auto ret = pal_screen[ppu_read(0x3F00 + (palette << 2) + pixel) & 0x3F];

	// if(ret.r != 'T' && ret.g != 'T' && ret.b != 'T') {
	// 	// LOG("Different color!!!"); // I tried to BP on this line lmao
	// }

	return ret;
	// return pal_screen[ppu_read(0x3F00 + (palette << 2) + pixel) & 0x3F];
}

olc::Sprite& ppu::get_screen() {
	return *spr_screen;
}

