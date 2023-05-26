#pragma once

#include <memory>
#include <string_view>

#include "parser.h"

#include "inesparser.h"
#include "nes2parser.h"

class ParserFactory
{
public:
    ParserFactory() = delete;
    void operator=(const ParserFactory&) = delete;

    static std::unique_ptr<Parser> create_parser(std::string_view file);

};