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
        if(dma_transfer) {
            if(dma_dummy) {
                if(system_clock_counter % 2 == 1) {
                    dma_dummy = false;
                }   
            } else {
                if(system_clock_counter % 2 == 0) {
                    dma_data = cpu_read((u16)(dma_page << 8) | dma_addr);
                } else {
                    ppu.p_oam[dma_addr] = dma_data;
                    dma_addr++;
                    if(dma_addr == 0x00) {
                        dma_transfer = false;
                        dma_dummy = true;
                    }
                }
            }
        } else cpu.clock();
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
    else if(addr >= 0x2000 && addr <= 0x3FFF) ppu.cpu_write(addr & 0x0007, data);
    else if(addr == 0x4014) {
        dma_page = data;
        dma_addr = 0x00;
        dma_transfer = true;
    }
    else if(addr >= 0x4016 && addr <= 0x4017) {
        controller_state[addr & 0x0001] = controller[addr & 0x0001];
    }
}

u8 mem_bus::cpu_read(u16 addr, bool readonly) {
    u8 data = 0x00;
    if (cart->cpu_read(addr, data)) {}
    else if(addr <= 0x1FFF) return cpu_ram[addr & 0x07FF];
    else if(addr >= 0x2000 && addr <= 0x3FFF) return ppu.cpu_read(addr & 0x0007, readonly);
    else if (addr >= 0x4016 && addr <= 0x4017) {
        // Read out the MSB of the controller status word
        data = (controller_state[addr & 0x0001] & 0x80) > 0;
        controller_state[addr & 0x0001] <<= 1;
    }

    return data;
}
