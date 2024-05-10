#pragma once

#include <memory>
#include <string_view>

#include "mapper/mapper.h"
#include "mapper/mapper_factory.h"
#include "memory/memory.h"

#include "component.h"

class Cartridge final : public Component
{
public:
    bool fetch(uint8_t &byte, uint16_t addr) override;
    bool commit(uint8_t byte, uint16_t addr) override;

    void service(Event event) override;

    std::string_view get_id() override { return "Cartridge"; }

    void load(const ROM &file);

private:
    Memory m_memory;
    std::shared_ptr<Mapper> m_mapper = nullptr;

    

};