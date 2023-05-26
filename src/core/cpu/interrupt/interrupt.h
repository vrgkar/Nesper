#pragma once

#include <string_view>
#include <cstdint>

#include "../register/register.h"

class CPU;

class Interrupt
{
public:
    virtual void execute(const CPU &cpu, Register &r) = 0;
    virtual std::string_view get_id() = 0;
};

class IRQ : public Interrupt
{
public:
    void execute(const CPU &cpu, Register &r) override;
    std::string_view get_id() { return "IRQ"; }
};

class NMI : public Interrupt
{
public:
    void execute(const CPU &cpu, Register &r) override;
    std::string_view get_id() { return "NMI"; }
};