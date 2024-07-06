//
// Created by user on 06.07.2024.
//

#ifndef MAPPER_H
#define MAPPER_H
#include "defines.h"

class mapper {
public:
    mapper(u8 prg_banks, u8 chr_banks) : prg_banks(prg_banks), chr_banks(chr_banks) {}
    virtual ~mapper() = default;

    virtual bool cpu_map_read(u16 addr, u32 &mapped_addr) = 0;
    virtual bool cpu_map_write(u16 addr, u32 &mapped_addr) = 0;
    virtual bool ppu_map_read(u16 addr, u32 &mapped_addr) = 0;
    virtual bool ppu_map_write(u16 addr, u32 &mapped_addr) = 0;

protected:
    u8 prg_banks = 0;
    u8 chr_banks = 0;
};

#endif //MAPPER_H
