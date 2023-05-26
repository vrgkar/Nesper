#pragma once

#include <array>
#include <cstdint>
#include <string_view>

#include "../system/component/component.h"

class RAM final : public Component
{
public:
    bool fetch(uint8_t &byte, uint16_t addr) override;
    bool commit(uint8_t byte, uint16_t addr) override;

    std::string_view get_id() override { return "RAM"; }

private:
    std::array<uint8_t, 2048> m_ram;
};
