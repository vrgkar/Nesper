#include "nes2parser.h"

void NES2Parser::load(ROM &rom, std::string_view file)
{
    rom.chrbanks = chrrom_banks(file);
    rom.prgbanks = prgrom_banks(file);

    rom.mapper_id = mapper_id(file);

    rom.trainer.assign(file.begin() + header_size(), file.begin() + header_size() + trainer_size(file));

    rom.prgrom.assign(
        file.begin() + header_size() + trainer_size(file),
        file.begin() + header_size() + trainer_size(file) + prgrom_size(file)
    );

    rom.chrrom.assign(
        file.begin() + header_size() + trainer_size(file) + prgrom_size(file), 
        file.begin() + header_size() + trainer_size(file) + prgrom_size(file) + chrrom_size(file)
    );
}

uint8_t NES2Parser::prgrom_lsb(std::string_view file)
{
    return file[4];
}

uint8_t NES2Parser::chrrom_lsb(std::string_view file)
{
    return file[5];
}

uint8_t NES2Parser::prgrom_msn(std::string_view file)
{
    return file[9] & 0x0Fu;
}

uint8_t NES2Parser::chrrom_msn(std::string_view file)
{
    return file[9] >> 4u;
}

uint16_t NES2Parser::prgrom_banks(std::string_view file)
{
    if (prgrom_msn(file) == 0xFu)
    {
        auto exp    = prgrom_lsb(file) >> 2u;
        auto mult   = prgrom_lsb(file) & 0x03u;
        return ((2u << exp) * mult) >> 14u;
    }
    else
        return prgrom_lsb(file) | (prgrom_msn(file) << 8u);
}

uint16_t NES2Parser::chrrom_banks(std::string_view file)
{
    if (chrrom_msn(file) == 0xFu)
    {
        auto exp    = chrrom_lsb(file) >> 2u;
        auto mult   = chrrom_lsb(file) & 0x03u;
        return ((2u << exp) * mult) >> 13u;
    }
    else
        return chrrom_lsb(file) | (chrrom_msn(file) << 8u);

}

uint32_t NES2Parser::prgrom_size(std::string_view file)
{
    if (prgrom_msn(file) == 0xFu)
    {
        auto exp    = prgrom_lsb(file) >> 2u;
        auto mult   = prgrom_lsb(file) & 0x03u;
        return ((2u << exp) * mult);
    }
    else
        return (prgrom_lsb(file) | (prgrom_msn(file) << 8u) << 14u);
}

uint32_t NES2Parser::chrrom_size(std::string_view file)
{
    if (chrrom_msn(file) == 0xFu)
    {
        auto exp    = chrrom_lsb(file) >> 2u;
        auto mult   = chrrom_lsb(file) & 0x03u;
        return ((2u << exp) * mult);
    }
    else
        return (chrrom_lsb(file) | (chrrom_msn(file) << 8u) << 13u);
}

int NES2Parser::mapper_id(std::string_view file)
{
    return ((file[8] & 0x0F) << 8) | (file[7] & 0xF0) | (file[6] >> 4);
}

int NES2Parser::trainer_size(std::string_view file)
{
    return (!!(file[6] & 0x4)) * 512;
}

