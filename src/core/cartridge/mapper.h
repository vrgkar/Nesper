#pragma once

#include <cstdint>
#include <string_view>

#include "../memory/memory.h"

class Mapper
{
public:
    /* Reads from an address, and returns whether it read the value into byte */
    virtual bool read(Memory &memory, uint8_t &byte, uint16_t addr) = 0;

    /* Writes to memory, and returns whether it wrote the value at the given address */
    virtual bool write(Memory &memory, uint8_t byte, uint16_t addr) = 0;

    virtual std::string_view get_id() const = 0;

    /* Mirroring Modes */
    enum class Mirroring
    {
        HORIZONTAL,
        VERTICAL
    };

    Mirroring mirroring() { return m_mirroring; }

    virtual ~Mapper() = default;

protected:
    Mirroring m_mirroring = Mirroring::HORIZONTAL;

    int m_prgbanks = 0;
    int m_chrbanks = 0;
};