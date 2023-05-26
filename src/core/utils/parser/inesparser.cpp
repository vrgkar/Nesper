#include "inesparser.h"

void INESParser::load(ROM &rom, std::string_view file)
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

uint16_t INESParser::prgrom_banks(std::string_view file)
{
    return file[4];
}

uint16_t INESParser::chrrom_banks(std::string_view file)
{
    return file[5];
}

uint32_t INESParser::prgrom_size(std::string_view file)
{
    return file[4] << 14u;
}

uint32_t INESParser::chrrom_size(std::string_view file)
{
    return file[5] << 13u;
}

int INESParser::mapper_id(std::string_view file)
{
    return (file[7] & 0xF0) | (file[6] >> 4);
}

int INESParser::trainer_size(std::string_view file)
{
    return !!(file[6] & 0x4) * 512;
}

