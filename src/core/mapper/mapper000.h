#pragma once

#include "mapper.h"

class Mapper000 final : public Mapper
{
public:
    bool read(uint8_t &byte, uint16_t addr) override;
    bool write(uint8_t byte, uint16_t addr) override;

    std::string_view get_id() const override { return "Mapper000"; }
private:
    
};