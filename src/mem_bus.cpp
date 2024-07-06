//
// Created by user on 06.07.2024.
//

#include "mem_bus.h"

mem_bus::mem_bus() {
    cpu.connect_bus(this);
}

mem_bus::~mem_bus() {

}

void mem_bus::write(u16 addr, u8 data) {
    if(addr >= 0x0000 && addr <= 0xFFFF) ram[addr] = data;
}

u8 mem_bus::read(u16 addr, bool readonly) {
    if(addr >= 0x0000 && addr <= 0xFFFF) return ram[addr];
    return 0x00;
}
