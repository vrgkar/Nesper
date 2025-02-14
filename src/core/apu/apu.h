#pragma once

#include "common/component.h"

class APU final : public Component
{
public:
    bool read(uint8_t &byte, uint16_t addr) override;
    bool write(uint8_t byte, uint16_t addr) override;

    void broadcast(Event event) override;
    void service(Event event) override;

    std::string_view get_id() const override { return "APU"; }
};