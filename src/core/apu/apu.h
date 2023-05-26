#pragma once

#include "../system/component/component.h"

class APU final : public Component
{
public:
    bool fetch(uint8_t &byte, uint16_t addr) override;
    bool commit(uint8_t byte, uint16_t addr) override;

    std::string_view get_id() override { return "APU"; }
};