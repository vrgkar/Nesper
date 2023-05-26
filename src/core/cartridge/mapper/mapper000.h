#pragma once

#include "mapper.h"

class Mapper000 final : public Mapper
{
public:
    bool read(Memory &memory, uint8_t &byte, uint16_t addr) override;
    bool write(Memory &memory, uint8_t byte, uint16_t addr) override;

    std::string_view get_id() const override { return "Mapper000"; }
private:
    
};