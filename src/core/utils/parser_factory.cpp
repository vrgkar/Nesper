#include "parser_factory.h"

std::unique_ptr<Parser> ParserFactory::create_parser(std::string_view file)
{
    if (file.substr(0, 3) == "NES" && (file[7] & 0xC0) != 0x80)
        return std::make_unique<INESParser>();
    else if (file.substr(0, 3) == "NES" && (file[7] & 0xC0) == 0x80)
        return std::make_unique<NES2Parser>();
    else
        return nullptr;
}