//
// Created by user on 06.07.2024.
//

#ifndef MEM_BUS_H
#define MEM_BUS_H

#include "cpu6502.h"
#include "defines.h"


class mem_bus {
public:
    mem_bus();
    ~mem_bus();

    void write(u16 addr, u8 data);
    u8 read(u16 addr, bool readonly = false);

    // Devices connected to the bus
    cpu6502 cpu;

    // Fake RAM
    u8 ram[64 * 1024]{};
};



#endif //MEM_BUS_H
