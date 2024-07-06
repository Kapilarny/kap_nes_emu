//
// Created by user on 06.07.2024.
//

#include "mapper_000.h"

bool mapper_000::cpu_map_read(u16 addr, u32 &mapped_addr) {
    if(addr >= 0x8000 && addr <= 0xFFFF) {
        mapped_addr = addr & (prg_banks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }

    return false;
}

bool mapper_000::cpu_map_write(u16 addr, u32 &mapped_addr) {
    if(addr >= 0x8000 && addr <= 0xFFFF) {
        mapped_addr = addr & (prg_banks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }

    return false;
}

bool mapper_000::ppu_map_read(u16 addr, u32 &mapped_addr) {
    if(addr >= 0x0000 && addr <= 0x1FFF) {
        mapped_addr = addr;
        return true;
    }

    return false;
}

bool mapper_000::ppu_map_write(u16 addr, u32 &mapped_addr) {
    // if(addr >= 0x0000 && addr <= 0x1FFF) {
    //     // mapped_addr = addr;
    //     return true;
    // }

    return false;
}
