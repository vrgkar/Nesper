#pragma once

#include <cstdint>

#include "../register/register.h"

class CPU;

class State
{
public:
    virtual void execute(CPU &cpu, Register &r) const = 0;
    virtual ~State() = default;
};

class IdleState final : public State
{
public:
    void execute(CPU &cpu, Register &r) const override;
};

class ReadyState final : public State
{
public:
    void execute(CPU &cpu, Register &r) const override;
};

class FetchState final : public State
{
public:
    void execute(CPU &cpu, Register &r) const override;
};

class DecodeState final : public State
{
public:
    void execute(CPU &cpu, Register &r) const override;
};

class ExecuteState final : public State
{
public:
    void execute(CPU &cpu, Register &r) const override;
};
