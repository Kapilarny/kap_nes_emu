#include <iostream>

#include "defines.h"
#include "logger.h"
// #include "emulator.h"

#define OLC_PGE_APPLICATION
#include <olcPixelGameEngine.h>
#include <xinput.h>

#include "gamepad.h"
#include "mem_bus.h"

class app : public olc::PixelGameEngine {
public:
    app() { sAppName = "Kap NES Emulator"; }

private:
    // Support Utilities
	std::map<uint16_t, std::string> mapAsm;

	std::string hex(uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};

	void DrawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns)
	{
		int nRamX = x, nRamY = y;
		for (int row = 0; row < nRows; row++)
		{
			std::string sOffset = "$" + hex(nAddr, 4) + ":";
			for (int col = 0; col < nColumns; col++)
			{
				sOffset += " " + hex(nes.cpu_read(nAddr, true), 2);
				nAddr += 1;
			}
			DrawString(nRamX, nRamY, sOffset);
			nRamY += 10;
		}
	}

	void DrawCpu(int x, int y)
	{
		std::string status = "STATUS: ";
		DrawString(x , y , "STATUS:", olc::WHITE);
		DrawString(x  + 64, y, "N", nes.cpu.status & cpu6502::N ? olc::GREEN : olc::RED);
		DrawString(x  + 80, y , "V", nes.cpu.status & cpu6502::V ? olc::GREEN : olc::RED);
		DrawString(x  + 96, y , "-", nes.cpu.status & cpu6502::U ? olc::GREEN : olc::RED);
		DrawString(x  + 112, y , "B", nes.cpu.status & cpu6502::B ? olc::GREEN : olc::RED);
		DrawString(x  + 128, y , "D", nes.cpu.status & cpu6502::D ? olc::GREEN : olc::RED);
		DrawString(x  + 144, y , "I", nes.cpu.status & cpu6502::I ? olc::GREEN : olc::RED);
		DrawString(x  + 160, y , "Z", nes.cpu.status & cpu6502::Z ? olc::GREEN : olc::RED);
		DrawString(x  + 178, y , "C", nes.cpu.status & cpu6502::C ? olc::GREEN : olc::RED);
		DrawString(x , y + 10, "PC: $" + hex(nes.cpu.pc, 4));
		DrawString(x , y + 20, "A: $" +  hex(nes.cpu.a, 2) + "  [" + std::to_string(nes.cpu.a) + "]");
		DrawString(x , y + 30, "X: $" +  hex(nes.cpu.x, 2) + "  [" + std::to_string(nes.cpu.x) + "]");
		DrawString(x , y + 40, "Y: $" +  hex(nes.cpu.y, 2) + "  [" + std::to_string(nes.cpu.y) + "]");
		DrawString(x , y + 50, "Stack P: $" + hex(nes.cpu.sp, 4));
	}

	void DrawCode(int x, int y, int nLines)
	{
		auto it_a = mapAsm.find(nes.cpu.pc);
		int nLineY = (nLines >> 1) * 10 + y;
		if (it_a != mapAsm.end())
		{
			DrawString(x, nLineY, (*it_a).second, olc::CYAN);
			while (nLineY < (nLines * 10) + y)
			{
				nLineY += 10;
				if (++it_a != mapAsm.end())
				{
					DrawString(x, nLineY, (*it_a).second);
				}
			}
		}

		it_a = mapAsm.find(nes.cpu.pc);
		nLineY = (nLines >> 1) * 10 + y;
		if (it_a != mapAsm.end())
		{
			while (nLineY > y)
			{
				nLineY -= 10;
				if (--it_a != mapAsm.end())
				{
					DrawString(x, nLineY, (*it_a).second);
				}
			}
		}
	}

	bool OnUserCreate() override
	{
		controller = gamepad();

		// Load the cartridge
		cart = std::make_shared<cartridge>("cart.nes");

		// Insert into NES
		nes.insert_cartridge(cart);

		// Extract dissassembly
		mapAsm = nes.cpu.disassemble(0x0000, 0xFFFF);

		// Reset NES
		nes.reset();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::DARK_BLUE);

		controller.refresh();

		bool aButton = GetKey(olc::Key::X).bHeld || controller.is_pressed(XINPUT_GAMEPAD_A);
		bool bButton = GetKey(olc::Key::Z).bHeld || controller.is_pressed(XINPUT_GAMEPAD_X);

		bool start_button = GetKey(olc::Key::A).bHeld || controller.is_pressed(XINPUT_GAMEPAD_START);
		bool select_button = GetKey(olc::Key::X).bHeld || controller.is_pressed(XINPUT_GAMEPAD_BACK);

		bool up_button = GetKey(olc::Key::UP).bHeld || controller.is_pressed(XINPUT_GAMEPAD_DPAD_UP);
		bool down_button = GetKey(olc::Key::DOWN).bHeld || controller.is_pressed(XINPUT_GAMEPAD_DPAD_DOWN);
		bool left_button = GetKey(olc::Key::LEFT).bHeld || controller.is_pressed(XINPUT_GAMEPAD_DPAD_LEFT);
		bool right_button = GetKey(olc::Key::RIGHT).bHeld || controller.is_pressed(XINPUT_GAMEPAD_DPAD_RIGHT);

		nes.controller[0] = 0x00;
		nes.controller[0] |= aButton ? 0x80 : 0x00;     // A Button
		nes.controller[0] |= bButton ? 0x40 : 0x00;     // B Button
		nes.controller[0] |= select_button ? 0x20 : 0x00;     // Select
		nes.controller[0] |= start_button ? 0x10 : 0x00;     // Start
		nes.controller[0] |= up_button ? 0x08 : 0x00;
		nes.controller[0] |= down_button ? 0x04 : 0x00;
		nes.controller[0] |= left_button ? 0x02 : 0x00;
		nes.controller[0] |= right_button ? 0x01 : 0x00;

		if (GetKey(olc::Key::SPACE).bPressed) emulation_run = !emulation_run;
		if (GetKey(olc::Key::R).bPressed) nes.reset();
		if (GetKey(olc::Key::P).bPressed) (++selected_palette) &= 0x07;

		if (emulation_run)
		{
			if (residual_time > 0.0f)
				residual_time -= fElapsedTime;
			else
			{
				residual_time += (1.0f / 60.0f) - fElapsedTime;
				do { nes.clock(); } while (!nes.ppu.frame_complete);
				nes.ppu.frame_complete = false;
			}
		}
		else
		{
			// Emulate code step-by-step
			if (GetKey(olc::Key::C).bPressed)
			{
				// Clock enough times to execute a whole CPU instruction
				do { nes.clock(); } while (!nes.cpu.complete());
				// CPU clock runs slower than system clock, so it may be
				// complete for additional system clock cycles. Drain
				// those out
				do { nes.clock(); } while (nes.cpu.complete());
			}

			// Emulate one whole frame
			if (GetKey(olc::Key::F).bPressed)
			{
				// Clock enough times to draw a single frame
				do { nes.clock(); } while (!nes.ppu.frame_complete);
				// Use residual clock cycles to complete current instruction
				do { nes.clock(); } while (!nes.cpu.complete());
				// Reset frame completion flag
				nes.ppu.frame_complete = false;
			}
		}

		DrawCpu(516, 2);
		// DrawCode(516, 72, 26);

		for(i32 i = 0; i < 26; i++) {
			std::string s = hex(i, 2) + ": (" + std::to_string(nes.ppu.p_oam[i * 4 + 3])
				+ ", " + std::to_string(nes.ppu.p_oam[i * 4 + 0]) + ") "
				+ "ID: " + hex(nes.ppu.p_oam[i * 4 + 1], 2)
				+ "AT: " + hex(nes.ppu.p_oam[i * 4 + 2], 2);

			DrawString(516, 72 + i * 10, s);
		}

		const int nSwatchSize = 6;
		for (int p = 0; p < 8; p++) // For each palette
			for(int s = 0; s < 4; s++) // For each index
				FillRect(516 + p * (nSwatchSize * 5) + s * nSwatchSize, 340,
					nSwatchSize, nSwatchSize, nes.ppu.get_color_from_palette_ram(p, s));

		DrawRect(516 + selected_palette * (nSwatchSize * 5) - 1, 339, (nSwatchSize * 4), nSwatchSize, olc::WHITE);

		DrawSprite(516, 348, &nes.ppu.get_pattern_table(0, selected_palette));
		DrawSprite(648, 348, &nes.ppu.get_pattern_table(1, selected_palette));

		DrawSprite(0, 0, &nes.ppu.get_screen(), 2);
		return true;
	}

	mem_bus nes;
	std::shared_ptr<cartridge> cart;
	bool emulation_run = false;
	f32 residual_time = 0.0f;

	u8 selected_palette = 0;
	gamepad controller;
};

int main() {
	app demo;
	demo.Construct(780, 480, 2, 2);
	demo.Start();

    return 0;
}
