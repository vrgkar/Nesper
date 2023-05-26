#pragma once

#include "parser.h"

class NES2Parser : public Parser
{
public:
    void load(ROM &rom, std::string_view file) override;

private:

    static consteval uint32_t header_size() { return 16u; }

    inline uint8_t prgrom_lsb(std::string_view file);
    inline uint8_t chrrom_lsb(std::string_view file);
    inline uint8_t prgrom_msn(std::string_view file);
    inline uint8_t chrrom_msn(std::string_view file);

    inline uint16_t prgrom_banks(std::string_view file);
    inline uint16_t chrrom_banks(std::string_view file);

    inline uint32_t prgrom_size(std::string_view file);
    inline uint32_t chrrom_size(std::string_view file);

    inline int mapper_id(std::string_view file);
    inline int trainer_size(std::string_view file);

};