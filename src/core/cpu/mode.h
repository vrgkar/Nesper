#pragma once

#include "common/pch.h"
#include "common/task.h"

#include "bus/bus.h"
#include "cpustate.h"

class Mode
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
    
    virtual ~Mode() = default;
};

class IMP final : public Mode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "IMP"; }
};

class IMM final : public Mode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "IMM"; }
};

class ZPG final : public Mode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "ZPG"; }
};

class ZPX final : public Mode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "ZPX"; }
};

class ZPY final : public Mode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "ZPY"; }
};

class ACC final : public Mode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "ACC"; }
};

class REL final : public Mode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "REL"; }
};

class ABS final : public Mode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "ABS"; }
};

class ABX final : public Mode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "ABX"; }
};

class ABY final : public Mode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "ABY"; }
};

class IND final : public Mode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "IND"; }
};

class IDX final : public Mode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "IDX"; }
};

class IDY final : public Mode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "IDY"; }
};

inline const Mode* const MODE_TABLE[12] =
{
    new IMP,
    new IMM,
    new ZPG,
    new ZPX,
    new ZPY,
    new ACC,
    new REL,
    new ABS,
    new ABX,
    new ABY,
    new IDX,
    new IDY
};
