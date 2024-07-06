//
// Created by user on 06.07.2024.
//

#ifndef MEM_BUS_H
#define MEM_BUS_H

#include <memory>

#include "cartridge.h"
#include "cpu6502.h"
#include "defines.h"
#include "ppu.h"


class mem_bus {
public:
    mem_bus();
    ~mem_bus();

    void insert_cartridge(const std::shared_ptr<cartridge>& cart);
    void reset();
    void clock();

    void cpu_write(u16 addr, u8 data);
    u8 cpu_read(u16 addr, bool readonly = false);

    // Devices connected to the bus
    cpu6502 cpu;
    u8 cpu_ram[2048]{};

    ppu ppu;

    std::shared_ptr<cartridge> cart;

    u8 controller[2]{};
private:
    u64 system_clock_counter = 0;
    u8 controller_state[2]{};

    u8 dma_page = 0x00;
    u8 dma_addr = 0x00;
    u8 dma_data = 0x00;

    bool dma_transfer = false;
    bool dma_dummy = true;
};



#endif //MEM_BUS_H
