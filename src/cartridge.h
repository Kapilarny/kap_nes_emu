//
// Created by user on 06.07.2024.
//

#ifndef CARTRIDGE_H
#define CARTRIDGE_H
#include <string>
#include <vector>

#include "defines.h"
#include "mappers/mapper.h"


class cartridge {
public:
    cartridge(const std::string& file_path);
    ~cartridge();

    bool cpu_read(u16 addr, u8 &data);
    bool cpu_write(u16 addr, u8 data);
    bool ppu_read(u16 addr, u8 &data);
    bool ppu_write(u16 addr, u8 data);

    enum MIRROR {
        HORIZONTAL,
        VERTICAL,
        ONESCREEN_LO,
        ONESCREEN_HI
    } mirror = HORIZONTAL;

private:
    u8 mapper_id = 0;
    u8 prg_banks = 0;
    u8 chr_banks = 0;

    std::vector<u8> prg_rom;
    std::vector<u8> chr_rom;

    mapper* mapper = nullptr;
};



#endif //CARTRIDGE_H
