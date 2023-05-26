#pragma once

#include <unordered_map>
#include <functional>


class Clock
{
public:
    void tick(CPU *cpu);
    void tick(PPU *ppu);
    void tick(APU *apu);



private:
    std::unordered_map<void*, std::function<void()>> m_callback;
};