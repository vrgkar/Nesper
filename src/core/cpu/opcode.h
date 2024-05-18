#pragma once

#include "common/pch.h"
#include "common/task.h"

#include "cpustate.h"
#include "bus/bus.h"

/* Base Instruction Class */
class Opcode
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
    
    virtual ~Opcode() = default;

};

inline const Opcode *hello = nullptr;

class ADC final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "ADC"; }
};

class AND final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "AND"; }
};

class ASL final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "ASL"; }
};

class BCC final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "BCC"; }
};

class BCS final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "BCS"; }
};

class BEQ final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "BEQ"; }
};

class BIT final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "BIT"; }
};

class BMI final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "BMI"; }
};

class BNE final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "BNE"; }
};

class BPL final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "BPL"; }
};

class BRK final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "BRK"; }
};

class BVC final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "BVC"; }
};

class BVS final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "BVS"; }
};

class CLC final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "CLC"; }
};

class CLD final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "CLD"; }
};

class CLI final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "CLI"; }
};

class CLV final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "CLV"; }
};

class CMP final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "CMP"; }
};

class CPX final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "CPX"; }
};

class CPY final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "CPY"; }
};

class DEC final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "DEC"; }
};

class DEX final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "DEX"; }
};

class DEY final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "DEY"; }
};

class EOR final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "EOR"; }
};

class INC final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "INC"; }
};

class INX final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "INX"; }
};

class INY final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "INY"; }
};

class JMP final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "JMP"; }
};

class JSR final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "JSR"; }
};

class LDA final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "LDA"; }
};

class LDX final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "LDX"; }
};

class LDY final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "LDY"; }
};

class LSR final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "LSR"; }
};

class NOP final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "NOP"; }
};

class ORA final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "ORA"; }
};

class PHA final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "PHA"; }
};

class PHP final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "PHP"; }
};

class PLA final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "PLA"; }
};

class PLP final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "PLP"; }
};

class ROL final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "ROL"; }
};

class ROR final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "ROR"; }
};

class RTI final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "RTI"; }
};

class RTS final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "RTS"; }
};

class SBC final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "SBC"; }
};

class SEC final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "SEC"; }
};

class SED final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "SED"; }
};

class SEI final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "SEI"; }
};

class STA final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "STA"; }
};

class STX final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "STX"; }
};

class STY final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "STY"; }
};

class TAX final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "TAX"; }
};

class TAY final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "TAY"; }
};

class TSX final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "TSX"; }
};

class TXA final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "TXA"; }
};

class TXS final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "TXS"; }
};

class TYA final : public Opcode
{
public:
    Task execute(Bus &bus, CPUState &s) const override;
    std::string_view get_id() const override { return "TYA"; }
};

inline const Opcode* const OPCODE_TABLE[56] =
{
    new ADC,
    new AND,
    new ASL,
    new BCC,
    new BCS,
    new BEQ,
    new BIT,
    new BMI,
    new BNE,
    new BPL,
    new BRK,
    new BVC,
    new BVS,
    new CLC,
    new CLD,
    new CLI,
    new CLV,
    new CMP,
    new CPX,
    new CPY,
    new DEC,
    new DEX,
    new DEY,
    new EOR,
    new INC,
    new INX,
    new INY,
    new JMP,
    new JSR,
    new LDA,
    new LDX,
    new LDY,
    new LSR,
    new NOP,
    new ORA,
    new PHA,
    new PHP,
    new PLA,
    new PLP,
    new ROL,
    new ROR,
    new RTI,
    new RTS,
    new SBC,
    new SEC,
    new SED,
    new SEI,
    new STA,
    new STX,
    new STY,
    new TAX,
    new TAY,
    new TSX,
    new TXA,
    new TXS,
    new TYA  
};