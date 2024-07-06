//
// Created by user on 06.07.2024.
//

#include "cartridge.h"

#include <fstream>

#include "mappers/impl/mapper_000.h"

cartridge::cartridge(const std::string &file_path) {
    struct header {
        char name[4];
        u8 prg_rom_chunks;
        u8 chr_rom_chunks;
        u8 mapper1;
        u8 mapper2;
        u8 prg_ram_size;
        u8 tv_system1;
        u8 tv_system2;
        char unused[5];
    } header;

    std::ifstream ifs(file_path, std::ifstream::binary);
    if(ifs.is_open()) {
        ifs.read((char*)&header, sizeof(header));

        if(header.mapper1 & 0x04) ifs.seekg(512, std::ios_base::cur);

        mapper_id = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
        mirror = (header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

        // file format
        u8 fileType = 1;

        if(fileType == 0) {}
        if(fileType == 1) {
            prg_banks = header.prg_rom_chunks;
            prg_rom.resize(prg_banks * 16384);
            ifs.read((char*)prg_rom.data(), prg_rom.size());

            chr_banks = header.chr_rom_chunks;
            chr_rom.resize(chr_banks * 8192);
            ifs.read((char*)chr_rom.data(), chr_rom.size());
        }
        if(fileType == 2) {}

        switch (mapper_id) {
            case 0: mapper = new mapper_000(prg_banks, chr_banks); break;
        }

        ifs.close();
    }
}

cartridge::~cartridge() {
    delete mapper;
}

bool cartridge::cpu_read(u16 addr, u8 &data) {
    u32 mapped_addr = 0;
    if(mapper->cpu_map_read(addr, mapped_addr)) {
        data = prg_rom[mapped_addr];
        return true;
    }

    return false;
}

bool cartridge::cpu_write(u16 addr, u8 data) {
    u32 mapped_addr = 0;
    if(mapper->cpu_map_write(addr, mapped_addr)) {
        prg_rom[mapped_addr] = data;
        return true;
    }

    return false;
}

bool cartridge::ppu_read(u16 addr, u8 &data) {
    u32 mapped_addr = 0;
    if(mapper->ppu_map_read(addr, mapped_addr)) {
        data = chr_rom[mapped_addr];
        return true;
    }

    return false;
}

bool cartridge::ppu_write(u16 addr, u8 data) {
    u32 mapped_addr = 0;
    if(mapper->ppu_map_write(addr, mapped_addr)) {
        chr_rom[mapped_addr] = data;
        return true;
    }

    return false;
}
