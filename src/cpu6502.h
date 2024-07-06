//
// Created by user on 06.07.2024.
//

#ifndef CPU_H
#define CPU_H

#include <map>
#include <string>
#include <vector>

#include "defines.h"

class mem_bus; // fwd declare

class cpu6502 {
public:
    cpu6502();
    ~cpu6502();

    void clock();
    void reset();
    void irq();
    void nmi();

    bool complete();

    u8 fetch();

    void connect_bus(mem_bus* n) { bus = n; }

    // Produces a map of strings, with keys equivalent to instruction start locations
    // in memory, for the specified range
    std::map<u16, std::string> disassemble(u16 start, u16 stop);

    // Cpu Stuff
    enum FLAGS {
        C = (1 << 0), // Carry Bit
        Z = (1 << 1), // Zero
        I = (1 << 2), // Disable Interrupts
        D = (1 << 3), // Decimal Mode (unused in this implementation)
        B = (1 << 4), // Break
        U = (1 << 5), // Unused
        V = (1 << 6), // Overflow
        N = (1 << 7), // Negative
    };

    u8 a = 0x00; // Accumulator Register
    u8 x = 0x00; // X Register
    u8 y = 0x00; // Y Register
    u8 sp = 0x00; // Stack Pointer
    u16 pc = 0x0000; // Program Counter
    u8 status = 0x00; // Status register

    // Addressing Modes
    u8 IMP(); u8 IMM(); u8 ZP0(); u8 ZPX(); u8 ZPY(); u8 REL(); u8 ABS(); u8 ABX(); u8 ABY(); u8 IND(); u8 IZX(); u8 IZY();

    // Opcodes
    u8 ADC(); u8 AND(); u8 ASL(); u8 BCC();
    u8 BCS(); u8 BEQ(); u8 BIT(); u8 BMI();
    u8 BNE(); u8 BPL(); u8 BRK(); u8 BVC();
    u8 BVS(); u8 CLC(); u8 CLD(); u8 CLI();
    u8 CLV(); u8 CMP(); u8 CPX(); u8 CPY();
    u8 DEC(); u8 DEX(); u8 DEY(); u8 EOR();
    u8 INC(); u8 INX(); u8 INY(); u8 JMP();
    u8 JSR(); u8 LDA(); u8 LDX(); u8 LDY();
    u8 LSR(); u8 NOP(); u8 ORA(); u8 PHA();
    u8 PHP(); u8 PLA(); u8 PLP(); u8 ROL();
    u8 ROR(); u8 RTI(); u8 RTS(); u8 SBC();
    u8 SEC(); u8 SED(); u8 SEI(); u8 STA();
    u8 STX(); u8 STY(); u8 TAX(); u8 TAY();
    u8 TSX(); u8 TXA(); u8 TXS(); u8 TYA();

    u8 XXX(); // Illegal opcode

private:
    mem_bus* bus = nullptr;

    u8 read(u16 addr);
    void write(u16 addr, u8 data);

    u8 get_flag(FLAGS f);
    void set_flag(FLAGS f, bool v);

    u8 fetched = 0x00;
    u16 addr_abs = 0x0000;
    u16 addr_rel = 0x00;
    u8 opcode = 0x00;
    u8 cycles = 0;

    u64 clock_count = 0;

    struct instruction {
        std::string name;
        u8(cpu6502::*operate)() = nullptr;
        u8(cpu6502::*addrmode)() = nullptr;
        u8 cycles = 0;
    };

    std::vector<instruction> lookup;
};



#endif //CPU_H
