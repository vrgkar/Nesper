#pragma once

#include "parser.h"

class INESParser : public Parser
{
public:
    void load(ROM &rom, std::string_view file) override;


private:
    static consteval uint32_t header_size() { return 16u; }

    inline uint16_t prgrom_banks(std::string_view file);
    inline uint16_t chrrom_banks(std::string_view file);

    inline uint32_t prgrom_size(std::string_view file);
    inline uint32_t chrrom_size(std::string_view file);

    inline int mapper_id(std::string_view file);
    inline int trainer_size(std::string_view file);
};