#pragma once

#include <string_view>
#include <cstdint>

#include "cpuregister.h"

class CPU;

class Interrupt
{
public:
    virtual void execute(const CPU &cpu, CPURegister &r) = 0;
    virtual std::string_view get_id() = 0;
};

class IRQ : public Interrupt
{
public:
    void execute(const CPU &cpu, CPURegister &r) override;
    std::string_view get_id() { return "IRQ"; }
};

class NMI : public Interrupt
{
public:
    void execute(const CPU &cpu, CPURegister &r) override;
    std::string_view get_id() { return "NMI"; }
};