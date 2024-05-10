#pragma once

#include "component.h"

class APU final : public Component
{
public:
    bool fetch(uint8_t &byte, uint16_t addr) override;
    bool commit(uint8_t byte, uint16_t addr) override;

    void service(Event event) override;

    std::string_view get_id() override { return "APU"; }
};