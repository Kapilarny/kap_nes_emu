//
// Created by user on 06.07.2024.
//

#ifndef MAPPER_000_H
#define MAPPER_000_H

#include "mappers/mapper.h"

class mapper_000 : public mapper {
public:
    mapper_000(u8 prg_banks, u8 chr_banks) : mapper(prg_banks, chr_banks) {}

    bool cpu_map_read(u16 addr, u32 &mapped_addr) override;
    bool cpu_map_write(u16 addr, u32 &mapped_addr) override;
    bool ppu_map_read(u16 addr, u32 &mapped_addr) override;
    bool ppu_map_write(u16 addr, u32 &mapped_addr) override;
};



#endif //MAPPER_000_H
