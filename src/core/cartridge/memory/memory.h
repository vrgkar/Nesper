#pragma once

#include <memory>
#include <vector>
#include <cstdint>

#include "common/rom.h"

class Memory
{
public:
    
    void load(const ROM &rom);

    constexpr auto prgrom_size() const { return m_prgrom.size(); }
    constexpr auto chrrom_size() const { return m_chrrom.size(); }

    constexpr auto prgram_size() const { return m_prgram.size(); }
    constexpr auto chrram_size() const { return m_chrram.size(); }

    constexpr auto prg_banks() const { return m_prgbanks; }
    constexpr auto chr_banks() const { return m_chrbanks; }

    uint8_t read_prgram(uint16_t addr) const;
    uint8_t read_prgrom(uint16_t addr) const;
    uint8_t read_chrram(uint16_t addr) const;
    uint8_t read_chrrom(uint16_t addr) const;

    void write_prgram(uint8_t byte, uint16_t addr) { m_prgram[addr % prgrom_size()] = byte; }
    void write_chrram(uint8_t byte, uint16_t addr) { m_chrram[addr & chrrom_size()] = byte; }

private:
    std::vector<uint8_t> m_prgrom;
    std::vector<uint8_t> m_chrrom;

    std::vector<uint8_t> m_prgram;
    std::vector<uint8_t> m_chrram;

    uint16_t m_prgbanks;
    uint16_t m_chrbanks;
};