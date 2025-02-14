#include "mapper_factory.h"
#include <iostream>

std::shared_ptr<Mapper> MapperFactory::create_mapper(const ROM &rom)
{
    switch (rom.mapper_id)
    {
        case 0:
            return std::make_shared<Mapper000>();
            break;
    }

    return nullptr;
}