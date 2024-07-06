//
// Created by user on 06.07.2024.
//

#include "cpu6502.h"

#include "mem_bus.h"

cpu6502::cpu6502() {
    using a = cpu6502;
	lookup =
	{
		{ "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
	};
}

cpu6502::~cpu6502() {
}

void cpu6502::clock() {
	if (cycles == 0) {
		opcode = read(pc);
		pc++;

		// Get starting number of cycles
		cycles = lookup[opcode].cycles;

		u8 add_cycle = (this->*lookup[opcode].addrmode)();
		u8 add_cycle2 = (this->*lookup[opcode].operate)();

		cycles += (add_cycle & add_cycle2);
	}

	cycles--;
}

void cpu6502::reset() {
	a = 0;
	x = 0;
	y = 0;
	sp = 0xFD;
	status = 0x00 | U;

	addr_abs = 0xFFFC;
	u16 low = read(addr_abs);
	u16 high = read(addr_abs + 1);

	pc = (high << 8) | low;

	addr_rel = 0;
	addr_abs = 0;
	fetched = 0;

	cycles = 8;
}

void cpu6502::irq() {
	if(get_flag(I) == 0) {
		write(0x0100 + sp, (pc >> 8) & 0x00FF);
		sp--;
		write(0x0100 + sp, pc & 0x00FF);
		sp--;

		set_flag(B, false);
		set_flag(U, true);
		set_flag(I, true);

		write(0x0100 + sp, status);
		sp--;

		addr_abs = 0xFFFE;
		u16 low = read(addr_abs);
		u16 high = read(addr_abs + 1);

		pc = (high << 8) | low;

		cycles = 7;
	}
}

void cpu6502::nmi() {
	write(0x0100 + sp, (pc >> 8) & 0x00FF);
	sp--;
	write(0x0100 + sp, pc & 0x00FF);
	sp--;

	set_flag(B, false);
	set_flag(U, true);
	set_flag(I, true);

	write(0x0100 + sp, status);
	sp--;

	addr_abs = 0xFFFA;
	u16 low = read(addr_abs);
	u16 high = read(addr_abs + 1);

	pc = (high << 8) | low;

	cycles = 7;
}

bool cpu6502::complete() {
	return cycles == 0;
}

u8 cpu6502::fetch() {
	if(lookup[opcode].addrmode != &cpu6502::IMP) fetched = read(addr_abs);

	return fetched;
}

u8 cpu6502::IMP() {
	fetched = a;
	return 0;
}

u8 cpu6502::IMM() {
	addr_abs = pc++;
	return 0;
}

u8 cpu6502::ZP0() {
	addr_abs = read(pc);
	pc++;
	addr_abs &= 0x00FF;

	return 0;
}

u8 cpu6502::ZPX() {
	addr_abs = (read(pc) + x);
	pc++;
	addr_abs &= 0x00FF;

	return 0;
}

u8 cpu6502::ZPY() {
	addr_abs = (read(pc) + y);
	pc++;
	addr_abs &= 0x00FF;

	return 0;
}

u8 cpu6502::REL() {
	addr_rel = read(pc);
	pc++;

	if(addr_rel & 0x80) addr_rel |= 0xFF00;

	return 0;
}

u8 cpu6502::ABS() {
	addr_abs = read(pc) | (read(pc + 1) << 8);
	pc += 2;

	return 0;
}

u8 cpu6502::ABX() {
	u16 low = read(pc);
	pc++;
	u16 high = read(pc);
	pc++;

	addr_abs = (high << 8) | low;
	addr_abs += x;

	if ((addr_abs & 0xFF00) != (high << 8)) return 1;

	return 0;
}

u8 cpu6502::ABY() {
	u16 low = read(pc);
	pc++;
	u16 high = read(pc);
	pc++;

	addr_abs = (high << 8) | low;
	addr_abs += y;

	if ((addr_abs & 0xFF00) != (high << 8)) return 1;

	return 0;
}

u8 cpu6502::IND() {
	u16 ptr_low = read(pc);
	pc++;
	u16 ptr_high = read(pc);
	pc++;

	u16 ptr = (ptr_high << 8) | ptr_low;

	if(ptr_low == 0x00FF) {
		// Simulate page boundary hardware bug
		addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
	} else {
		// Normal behavior
		addr_abs = (read(ptr + 1) << 8) | read(ptr + 0);
	}

	return 0;
}

u8 cpu6502::IZX() {
	u16 t = read(pc);
	pc++;

	u16 lo = read((u16)(t + (u16)x) & 0x00FF);
	u16 hi = read((u16)(t + (u16)x + 1) & 0x00FF);

	addr_abs = (hi << 8) | lo;

	return 0;
}

u8 cpu6502::IZY() {
	u16 t = read(pc);
	pc++;

	u16 lo = read(t & 0x00FF);
	u16 hi = read((t + 1) & 0x00FF);

	addr_abs = (hi << 8) | lo;
	addr_abs += y;

	if((addr_abs & 0xFF00) != (hi << 8)) return 1;

	return 0;
}

u8 cpu6502::AND() {
	fetch();

	a = a & fetched;
	set_flag(Z, a == 0x00);
	set_flag(N, a & 0x80);

	return 1;
}

u8 cpu6502::ASL() {
	fetch();

	u16 temp = (u16)fetched << 1;
	set_flag(C, (temp & 0xFF00) > 0);
	set_flag(Z, (temp & 0x00FF) == 0x00);
	set_flag(N, temp & 0x80);

	if(lookup[opcode].addrmode == &cpu6502::IMP) a = temp & 0x00FF;
	else write(addr_abs, temp & 0x00FF);

	return 0;
}

u8 cpu6502::BCC() {
	if(get_flag(C) == 0) {
		cycles++;
		addr_abs = pc + addr_rel;

		if((addr_abs & 0xFF00) != (pc & 0xFF00)) cycles++;

		pc = addr_abs;
	}

	return 0;
}

u8 cpu6502::BCS() {
	if(get_flag(C) == 1) {
		cycles++;
		addr_abs = pc + addr_rel;

		if((addr_abs & 0xFF00) != (pc & 0xFF00)) cycles++;

		pc = addr_abs;
	}

	return 0;
}

u8 cpu6502::BEQ() {
	if(get_flag(Z) == 1) {
		cycles++;
		addr_abs = pc + addr_rel;

		if((addr_abs & 0xFF00) != (pc & 0xFF00)) cycles++;

		pc = addr_abs;
	}

	return 0;
}

u8 cpu6502::BIT() {
	fetch();

	const u16 temp = a & fetched;
	set_flag(Z, (temp & 0x00FF) == 0x00);
	set_flag(N, fetched & (1 << 7));
	set_flag(V, fetched & (1 << 6));

	return 0;
}

u8 cpu6502::BMI() {
	if(get_flag(N) == 1) {
		cycles++;
		addr_abs = pc + addr_rel;

		if((addr_abs & 0xFF00) != (pc & 0xFF00)) cycles++;

		pc = addr_abs;
	}

	return 0;
}

u8 cpu6502::BNE() {
	if(get_flag(Z) == 0) {
		cycles++;
		addr_abs = pc + addr_rel;

		if((addr_abs & 0xFF00) != (pc & 0xFF00)) cycles++;

		pc = addr_abs;
	}

	return 0;
}

u8 cpu6502::BPL() {
	if(get_flag(N) == 0) {
		cycles++;
		addr_abs = pc + addr_rel;

		if((addr_abs & 0xFF00) != (pc & 0xFF00)) cycles++;

		pc = addr_abs;
	}

	return 0;
}

u8 cpu6502::BRK() {
	pc++;

	set_flag(I, true);
	write(0x0100 + sp, (pc >> 8) & 0x00FF);
	sp--;
	write(0x0100 + sp, pc & 0x00FF);
	sp--;

	set_flag(B, 1);
	write(0x0100 + sp, status);
	sp--;
	set_flag(B, 0);

	pc = (u16)read(0xFFFE) | ((u16)read(0xFFFF) << 8);
	return 0;
}

u8 cpu6502::BVC() {
	if(get_flag(V) == 0) {
		cycles++;
		addr_abs = pc + addr_rel;

		if((addr_abs & 0xFF00) != (pc & 0xFF00)) cycles++;

		pc = addr_abs;
	}

	return 0;
}

u8 cpu6502::BVS() {
	if(get_flag(V) == 1) {
		cycles++;
		addr_abs = pc + addr_rel;

		if((addr_abs & 0xFF00) != (pc & 0xFF00)) cycles++;

		pc = addr_abs;
	}

	return 0;
}

u8 cpu6502::CLC() {
	set_flag(C, false);
	return 0;
}

u8 cpu6502::CLD() {
	set_flag(D, false);
	return 0;
}

u8 cpu6502::CLI() {
	set_flag(I, false);
	return 0;
}

u8 cpu6502::CLV() {
	set_flag(V, false);
	return 0;
}

u8 cpu6502::CMP() {
	fetch();

	const u16 temp = (u16)a - (u16)fetched;
	set_flag(C, a >= fetched);
	set_flag(Z, (temp & 0x00FF) == 0x00);
	set_flag(N, temp & 0x80);

	return 1;
}

u8 cpu6502::CPX() {
	fetch();

	const u16 temp = (u16)x - (u16)fetched;
	set_flag(C, x >= fetched);
	set_flag(Z, (temp & 0x00FF) == 0x00);
	set_flag(N, temp & 0x80);

	return 0;
}

u8 cpu6502::CPY() {
	fetch();

	const u16 temp = (u16)y - (u16)fetched;
	set_flag(C, y >= fetched);
	set_flag(Z, (temp & 0x00FF) == 0x00);
	set_flag(N, temp & 0x80);

	return 0;
}

u8 cpu6502::DEC() {
	fetch();

	const u16 temp = fetched - 1;
	write(addr_abs, temp & 0x00FF);
	set_flag(Z, (temp & 0x00FF) == 0x00);
	set_flag(N, temp & 0x80);

	return 0;
}

u8 cpu6502::DEX() {
	x--;
	set_flag(Z, x == 0x00);
	set_flag(N, x & 0x80);

	return 0;
}

u8 cpu6502::DEY() {
	y--;
	set_flag(Z, y == 0x00);
	set_flag(N, y & 0x80);

	return 0;
}

u8 cpu6502::EOR() {
	fetch();

	a = a ^ fetched;
	set_flag(Z, a == 0x00);
	set_flag(N, a & 0x80);

	return 1;
}

u8 cpu6502::INC() {
	fetch();
	const u16 temp = fetched + 1;
	write(addr_abs, temp & 0x00FF);

	set_flag(Z, (temp & 0x00FF) == 0x00);
	set_flag(N, temp & 0x80);

	return 0;
}

u8 cpu6502::INX() {
	x++;
	set_flag(Z, x == 0x00);
	set_flag(N, x & 0x80);

	return 0;
}

u8 cpu6502::INY() {
	y++;

	set_flag(Z, y == 0x00);
	set_flag(N, y & 0x80);

	return 0;
}

u8 cpu6502::JMP() {
	pc = addr_abs;
	return 0;
}

u8 cpu6502::JSR() {
	pc--;

	write(0x0100 + sp, (pc >> 8) & 0x00FF);
	sp--;
	write(0x0100 + sp, pc & 0x00FF);
	sp--;

	pc = addr_abs;

	return 0;
}

u8 cpu6502::LDA() {
	fetch();

	a = fetched;
	set_flag(Z, a == 0x00);
	set_flag(N, a & 0x80);

	return 1;
}

u8 cpu6502::LDX() {
	fetch();

	x = fetched;

	set_flag(Z, x == 0x00);
	set_flag(N, x & 0x80);

	return 1;
}

u8 cpu6502::LDY() {
	fetch();

	y = fetched;
	set_flag(Z, y == 0x00);
	set_flag(N, y & 0x80);

	return 1;
}

u8 cpu6502::LSR() {
	fetch();

	set_flag(C, fetched & 0x0001);
	const u16 temp = fetched >> 1;

	set_flag(Z, (temp & 0x00FF) == 0x00);
	set_flag(N, temp & 0x0080);

	if(lookup[opcode].addrmode == &cpu6502::IMP) a = temp & 0x00FF;
	else write(addr_abs, temp & 0x00FF);

	return 0;
}

u8 cpu6502::NOP() {
	// Sadly not all NOPs are equal
	// based on https://wiki.nesdev.com/w/index.php/CPU_unofficial_opcodes
	switch (opcode) {
		case 0x1C:
		case 0x3C:
		case 0x5C:
		case 0x7C:
		case 0xDC:
		case 0xFC:
			return 1;
		default: return 0;
	}
}

u8 cpu6502::ORA() {
	fetch();

	a = a | fetched;

	set_flag(Z, a == 0x00);
	set_flag(N, a & 0x80);

	return 1;
}

u8 cpu6502::PHA() {
	write(0x0100 + sp, a);
	sp--;

	return 0;
}

u8 cpu6502::PLA() {
	sp++;
	a = read(0x0100 + sp);

	set_flag(Z, a == 0x00);
	set_flag(N, a & 0x80);

	return 0;

}

u8 cpu6502::PHP() {
	write(0x0100 + sp, status | B | U);

	set_flag(B, false);
	set_flag(U, false);

	sp--;

	return 0;
}

u8 cpu6502::PLP() {
	sp++;

	status = read(0x0100 + sp);
	set_flag(U, true);

	return 0;
}

u8 cpu6502::ROL() {
	fetch();

	const u16 temp = (u16)(fetched << 1) | get_flag(C);
	set_flag(C, temp & 0xFF00);
	set_flag(Z, (temp & 0x00FF) == 0x00);
	set_flag(N, temp & 0x80);

	if(lookup[opcode].addrmode == &cpu6502::IMP) a = temp & 0x00FF;
	else write(addr_abs, temp & 0x00FF);

	return 0;
}

u8 cpu6502::ROR() {
	fetch();

	const u16 temp = (u16)(get_flag(C) << 7) | (fetched >> 1);
	set_flag(C, fetched & 0x01);
	set_flag(Z, (temp & 0x00FF) == 0x00);
	set_flag(N, temp & 0x80);

	if(lookup[opcode].addrmode == &cpu6502::IMP) a = temp & 0x00FF;
	else write(addr_abs, temp & 0x00FF);

	return 0;
}

u8 cpu6502::RTI() {
	sp++;
	status = read(0x0100 + sp);
	status &= ~B;
	status &= ~U;

	sp++;
	pc = (u16)read(0x0100 + sp);

	sp++;
	pc |= (u16)read(0x0100 + sp) << 8;

	return 0;
}

u8 cpu6502::RTS() {
	sp++;
	pc = (u16)read(0x0100 + sp);
	sp++;
	pc |= (u16)read(0x0100 + sp) << 8;

	pc++;

	return 0;
}

u8 cpu6502::ADC() {
	fetch();

	// Add is performed in 16-bit domain for emulation to capture any
	// carry bit, which will exist in bit 8 of the 16-bit word
	const u16 temp = (u16)a + (u16)fetched + (u16)get_flag(C);

	// Carry flag exists in the high byte bit 0
	set_flag(C, temp > 255);

	// Zero flag if res is 0
	set_flag(Z, (temp & 0x00FF) == 0);

	// Negative is set to the most significant bit of the result
	set_flag(N, temp & 0x80);

	// Overflow flag, its a mess lmao
	set_flag(V, (~(u16)a ^ (u16)fetched) & ((u16)a ^ temp) & 0x0080);

	// Load the 8-bit result into the accumulator
	a = temp & 0x00FF;

	return 1;
}

u8 cpu6502::SBC() {
	fetch();

	u16 value = (u16)fetched ^ 0x00FF;
	u16 temp = (u16)a + value + (u16)get_flag(C);

	set_flag(C, temp & 0xFF00);
	set_flag(Z, (temp & 0x00FF) == 0);
	set_flag(N, temp & 0x80);
	set_flag(V, (~(u16)a ^ (u16)fetched) & ((u16)a ^ temp) & 0x0080);

	a = temp & 0x00FF;

	return 1;
}

u8 cpu6502::SEC() {
	set_flag(C, true);

	return 0;
}

u8 cpu6502::SED() {
	set_flag(D, true);

	return 0;
}

u8 cpu6502::SEI() {
	set_flag(I, true);

	return 0;
}

u8 cpu6502::STA() {
	write(addr_abs, a);
	return 0;
}

u8 cpu6502::STX() {
	write(addr_abs, x);
	return 0;
}

u8 cpu6502::STY() {
	write(addr_abs, y);
	return 0;
}

u8 cpu6502::TAX() {
	x = a;

	set_flag(Z, x == 0x00);
	set_flag(N, x & 0x80);

	return 0;
}

u8 cpu6502::TAY() {
	y = a;

	set_flag(Z, y == 0x00);
	set_flag(N, y & 0x80);

	return 0;
}

u8 cpu6502::TSX() {
	x = sp;
	set_flag(Z, x == 0x00);
	set_flag(N, x & 0x80);

	return 0;
}

u8 cpu6502::TXA() {
	a = x;

	set_flag(Z, a == 0x00);
	set_flag(N, a & 0x80);

	return 0;
}

u8 cpu6502::TXS() {
	sp = x;

	return 0;
}

u8 cpu6502::TYA() {
	a = y;

	set_flag(Z, a == 0x00);
	set_flag(N, a & 0x80);

	return 0;
}

u8 cpu6502::XXX() {
	return 0;
}

u8 cpu6502::get_flag(FLAGS f) {
	return (status & f) > 0 ? 1 : 0;
}

void cpu6502::set_flag(FLAGS f, bool v) {
	if(v) status |= f;
	else status &= ~f;
}

u8 cpu6502::read(u16 addr) {
    return bus->cpu_read(addr, false);
}

void cpu6502::write(u16 addr, u8 data) {
    bus->cpu_write(addr, data);
}

std::map<u16, std::string> cpu6502::disassemble(u16 start, u16 stop) {
	u32 addr = start;
	u8 value = 0x00, low = 0x00, high = 0x00;
	std::map<u16, std::string> mapLines;
	u16 line_addr = 0;

	auto hex = [](u32 n, u8 d) {
		std::string s(d, '0');
		for(int i = d - 1; i >= 0; i--, n >>= 4) s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};

	while(addr <= (u32)stop) {
		line_addr = addr;

		// Prefix
		std::string sInst = "$" + hex(addr, 4) + ": ";

		// Read opcode
		u8 opcode = bus->cpu_read(addr, true); addr++;
		auto& inst = lookup[opcode];
		sInst += inst.name + " ";

		if (inst.addrmode == &cpu6502::IMP) {
			sInst += " {IMP}";
		} else if (inst.addrmode == &cpu6502::IMM) {
			value = bus->cpu_read(addr, true); addr++;
			sInst += "#$" + hex(value, 2) + " {IMM}";
		} else if (inst.addrmode == &cpu6502::ZP0) {
			low = bus->cpu_read(addr, true); addr++;
			high = 0x00;
			sInst += "$" + hex(low, 2) + " {ZP0}";
		} else if (inst.addrmode == &cpu6502::ZPX) {
			low = bus->cpu_read(addr, true); addr++;
			high = 0x00;
			sInst += "$" + hex(low, 2) + ", X {ZPX}";
		} else if (inst.addrmode == &cpu6502::ZPY) {
			low = bus->cpu_read(addr, true); addr++;
			high = 0x00;
			sInst += "$" + hex(low, 2) + ", Y {ZPY}";
		} else if (inst.addrmode == &cpu6502::IZX) {
			low = bus->cpu_read(addr, true); addr++;
			high = 0x00;
			sInst += "($" + hex(low, 2) + ", X) {IZX}";
		} else if (inst.addrmode == &cpu6502::IZY) {
			low = bus->cpu_read(addr, true); addr++;
			high = 0x00;
			sInst += "($" + hex(low, 2) + "), Y {IZY}";
		} else if (inst.addrmode == &cpu6502::ABS) {
			low = bus->cpu_read(addr, true); addr++;
			high = bus->cpu_read(addr, true); addr++;
			sInst += "$" + hex((u16)(high << 8) | low, 4) + " {ABS}";
		} else if (inst.addrmode == &cpu6502::ABX) {
			low = bus->cpu_read(addr, true); addr++;
			high = bus->cpu_read(addr, true); addr++;
			sInst += "$" + hex((u16)(high << 8) | low, 4) + ", X {ABX}";
		} else if (inst.addrmode == &cpu6502::ABY) {
			low = bus->cpu_read(addr, true); addr++;
			high = bus->cpu_read(addr, true); addr++;
			sInst += "$" + hex((u16)(high << 8) | low, 4) + ", Y {ABY}";
		} else if (inst.addrmode == &cpu6502::IND) {
			low = bus->cpu_read(addr, true); addr++;
			high = bus->cpu_read(addr, true); addr++;
			sInst += "($" + hex((u16)(high << 8) | low, 4) + ") {IND}";
		} else if (inst.addrmode == &cpu6502::REL) {
			value = bus->cpu_read(addr, true); addr++;
			sInst += "$" + hex(value, 2) + " [$" + hex(addr + (i8)value, 4) + "] {REL}";
		}

		mapLines[line_addr] = sInst;
	}

	return mapLines;
}