#pragma once

#include <cstdint>
#include <vector>

struct ROM
{
    int mapper_id;
    int submapper_id;

    bool mirroring;

    uint16_t prgbanks = 0;
    uint16_t chrbanks = 0;

    std::vector<uint8_t> trainer;

    std::vector<uint8_t> prgrom;
    std::vector<uint8_t> chrrom;

};