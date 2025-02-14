#pragma once

#include "common/pch.h"
#include "common/task.h"

#include "bus/bus.h"
#include "cpustate.h"

class Interrupt
{
public:
    virtual Task execute(Bus &bus, CPUState &s) const = 0;
    virtual std::string_view get_id() const = 0;

    bool step(Bus &bus, CPUState &s) const
    {
        static auto task = execute(bus, s);

        task();

        if (task.is_done())
        {
            task = execute(bus, s);
            return false;
        }

        return true;
    }
    
    virtual ~Interrupt() = default;
};

class RST : public Interrupt
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "RST"; }
};

class IRQ : public Interrupt
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "IRQ"; }
};

class NMI : public Interrupt
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "NMI"; }
};

inline const Interrupt* const INTERRUPT_TABLE[3]
{
    new RST,
    new IRQ,
    new NMI,
};