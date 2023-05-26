#pragma once

#include <string_view>
#include <coroutine>

#include "../register/register.h"

class CPU;

struct OpcodeTask {
    // The coroutine level type
    struct promise_type {
        using Handle = std::coroutine_handle<promise_type>;
        void return_value(int value) { current_value = value; }
        OpcodeTask get_return_object() {
            return OpcodeTask{Handle::from_promise(*this)};
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(int value) {
            current_value = value;
            return {};
        }
        void unhandled_exception() { }
        int current_value;
    };
    explicit OpcodeTask(promise_type::Handle coro) : coro_(coro) {}
    ~OpcodeTask() {
        if (coro_) coro_.destroy();
    }
    // Make move-only
    OpcodeTask(const OpcodeTask&) = delete;
    OpcodeTask& operator=(const OpcodeTask&) = delete;
    OpcodeTask(OpcodeTask&& t) noexcept : coro_(t.coro_) { 
        t.coro_ = {};
    }
    OpcodeTask& operator=(OpcodeTask&& t) noexcept {
        if (this == &t) return *this;
        if (coro_) coro_.destroy();
        coro_ = t.coro_;
        t.coro_ = {};
        return *this;
    }
    auto& operator()() noexcept
    {
        coro_.resume();
        return get_value();
    }

    int& get_value() noexcept
    {
        return coro_.promise().current_value;
    }
    bool is_done()
    {
        return coro_.done();
    }
private:
    promise_type::Handle coro_;
};

/* Base Instruction Class */
class Opcode
{
public:
    virtual OpcodeTask execute(CPU &cpu, Register &r) const = 0;
    virtual std::string_view get_id() const = 0;

    bool step(CPU &cpu, Register &r)
    {
        static auto task = execute(cpu, r);

        if (m_reset)
        {
            task = execute(cpu, r);
            m_reset = false;
        }

        if (task.is_done())
            return false;

        task();
        return !task.is_done();
    }
    
    virtual ~Opcode() = default;

private:
    bool m_reset = true;
};

class ADC final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "ADC"; }
};

class AND final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "AND"; }
};

class ASL final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "ASL"; }
};

class BCC final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "BCC"; }
};

class BCS final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "BCS"; }
};

class BEQ final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "BEQ"; }
};

class BIT final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "BIT"; }
};

class BMI final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "BMI"; }
};

class BNE final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "BNE"; }
};

class BPL final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "BPL"; }
};

class BRK final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "BRK"; }
};

class BVC final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "BVC"; }
};

class BVS final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "BVS"; }
};

class CLC final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "CLC"; }
};

class CLD final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "CLD"; }
};

class CLI final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "CLI"; }
};

class CLV final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "CLV"; }
};

class CMP final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "CMP"; }
};

class CPX final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "CPX"; }
};

class CPY final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "CPY"; }
};

class DEC final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "DEC"; }
};

class DEX final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "DEX"; }
};

class DEY final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "DEY"; }
};

class EOR final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "EOR"; }
};

class INC final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "INC"; }
};

class INX final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "INX"; }
};

class INY final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "INY"; }
};

class JMP final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "JMP"; }
};

class JSR final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "JSR"; }
};

class LDA final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "LDA"; }
};

class LDX final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "LDX"; }
};

class LDY final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "LDY"; }
};

class LSR final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "LSR"; }
};

class NOP final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "NOP"; }
};

class ORA final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "ORA"; }
};

class PHA final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "PHA"; }
};

class PHP final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "PHP"; }
};

class PLA final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "PLA"; }
};

class PLP final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "PLP"; }
};

class ROL final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "ROL"; }
};

class ROR final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "ROR"; }
};

class RTI final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "RTI"; }
};

class RTS final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "RTS"; }
};

class SBC final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "SBC"; }
};

class SEC final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "SEC"; }
};

class SED final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "SED"; }
};

class SEI final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "SEI"; }
};

class STA final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "STA"; }
};

class STX final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "STX"; }
};

class STY final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "STY"; }
};

class TAX final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "TAX"; }
};

class TAY final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "TAY"; }
};

class TSX final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "TSX"; }
};

class TXA final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "TXA"; }
};

class TXS final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "TXS"; }
};

class TYA final : public Opcode
{
public:
    OpcodeTask execute(CPU &cpu, Register &r) const override;
    std::string_view get_id() const override { return "TYA"; }
};