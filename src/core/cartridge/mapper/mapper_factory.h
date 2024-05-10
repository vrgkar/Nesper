#pragma once

#include <cstdint>

#include "common/rom.h"

#include "mapper.h"
#include "mapper000.h"

class MapperFactory
{
public:
    MapperFactory() = delete;
    void operator=(const MapperFactory&) = delete;

    static std::shared_ptr<Mapper> create_mapper(const ROM &rom);
};