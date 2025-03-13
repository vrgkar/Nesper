#pragma once

#include "mapper.h"

class Mapper000 final : public Mapper
{
public:
    uint8_t read(uint16_t addr) override;
    void    write(uint8_t byte, uint16_t addr) override;

    std::string_view get_id() const override { return "Mapper000"; }    
};