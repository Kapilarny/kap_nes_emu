//
// Created by user on 06.07.2024.
//

#include "mem_bus.h"

mem_bus::mem_bus() {
    cpu.connect_bus(this);
}

mem_bus::~mem_bus() {

}

void mem_bus::insert_cartridge(const std::shared_ptr<cartridge>& cart) {
    this->cart = cart;
    ppu.connect_cartridge(cart);
}

void mem_bus::reset() {
    cpu.reset();
    system_clock_counter = 0;
}

void mem_bus::clock() {
    ppu.clock();
    if(system_clock_counter % 3 == 0) {
        cpu.clock();
    }

    if(ppu.nmi) {
        ppu.nmi = false;
        cpu.nmi();
    }

    system_clock_counter++;
}

void mem_bus::cpu_write(u16 addr, u8 data) {
    if(cart->cpu_write(addr, data)) {}
    else if(addr <= 0x1FFF) cpu_ram[addr & 0x07FF] = data;
    else if(addr >= 0x2000 && addr <= 0x3FFF) return ppu.cpu_write(addr & 0x0007, data);
}

u8 mem_bus::cpu_read(u16 addr, bool readonly) {
    u8 data = 0x00;
    if (cart->cpu_read(addr, data)) {}
    else if(addr <= 0x1FFF) return cpu_ram[addr & 0x07FF];
    else if(addr >= 0x2000 && addr <= 0x3FFF) return ppu.cpu_read(addr & 0x0007, readonly);

    return data;
}
