#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "common/rom.h"

class Parser
{
public:
    virtual void load(ROM &rom, std::string_view file) = 0;
    virtual ~Parser() = default;
};