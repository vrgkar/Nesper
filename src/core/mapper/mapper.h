#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

#include "common/component.h"
#include "common/rom.h"
#include "bus/bus.h"

class Mapper : public Component
{
public:
    /* Reads from an address, and returns whether it read the value into byte */
    virtual uint8_t read(uint16_t addr) = 0;

    /* Writes to memory, and returns whether it wrote the value at the given address */
    virtual void write(uint8_t byte, uint16_t addr) = 0;

    /* Returns the ID of the mapper */
    virtual std::string_view get_id() const = 0;

    void connect(Bus *bus) { m_bus = bus; m_bus->connect(this); }

    virtual ~Mapper() = default;

public:

    /* Mirroring Modes */
    enum class Mirroring
    {
        HORIZONTAL,
        VERTICAL
    };

    Mirroring mirroring() { return m_mirroring; }

    void load_rom(const ROM &rom);

protected:

    uint8_t read_prgram(uint16_t addr) const;
    uint8_t read_prgrom(uint16_t addr) const;
    uint8_t read_chrram(uint16_t addr) const;
    uint8_t read_chrrom(uint16_t addr) const;

    void write_prgram(uint8_t byte, uint16_t addr);
    void write_chrram(uint8_t byte, uint16_t addr);

protected:
    Mirroring m_mirroring = Mirroring::HORIZONTAL;

    uint16_t m_prgbanks = 0;
    uint16_t m_chrbanks = 0;

    std::vector<uint8_t> m_prgrom;
    std::vector<uint8_t> m_chrrom;

    std::vector<uint8_t> m_prgram;
    std::vector<uint8_t> m_chrram;

    std::array<uint8_t, 0x800> m_vram;

    Bus *m_bus = nullptr;

};