#pragma once

#include <string_view>
#include <coroutine>
#include <variant>

#include "cpuregister.h"

class CPU;

struct ModeTask {
    // The coroutine level type
    struct promise_type {
        using Handle = std::coroutine_handle<promise_type>;
        void return_value(int value) { current_value = value; }
        ModeTask get_return_object() {
            return ModeTask{Handle::from_promise(*this)};
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
    explicit ModeTask(promise_type::Handle coro) : coro_(coro) {}
    ~ModeTask() {
        if (coro_) coro_.destroy();
    }
    // Make move-only
    ModeTask(const ModeTask&) = delete;
    ModeTask& operator=(const ModeTask&) = delete;
    ModeTask(ModeTask&& t) noexcept : coro_(t.coro_) { 
        t.coro_ = {};
    }
    ModeTask& operator=(ModeTask&& t) noexcept {
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

class Mode
{
public:
    virtual ModeTask execute(CPU &cpu, CPURegister &r) const = 0;
    virtual std::string_view get_id() const = 0;

    bool step(CPU &cpu, CPURegister &r)
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
    
    virtual ~Mode() = default;

private:
    bool m_reset = true;
};

class IMP final : public Mode
{
public:
    ModeTask execute(CPU &cpu, CPURegister &r) const override;
    std::string_view get_id() const override { return "IMP"; }
};

class IMM final : public Mode
{
public:
    ModeTask execute(CPU &cpu, CPURegister &r) const override;
    std::string_view get_id() const override { return "IMM"; }
};

class ZPG final : public Mode
{
public:
    ModeTask execute(CPU &cpu, CPURegister &r) const override;
    std::string_view get_id() const override { return "ZPG"; }
};

class ZPX final : public Mode
{
public:
    ModeTask execute(CPU &cpu, CPURegister &r) const override;
    std::string_view get_id() const override { return "ZPX"; }
};

class ZPY final : public Mode
{
public:
    ModeTask execute(CPU &cpu, CPURegister &r) const override;
    std::string_view get_id() const override { return "ZPY"; }
};

class ACC final : public Mode
{
public:
    ModeTask execute(CPU &cpu, CPURegister &r) const override;
    std::string_view get_id() const override { return "ACC"; }
};

class REL final : public Mode
{
public:
    ModeTask execute(CPU &cpu, CPURegister &r) const override;
    std::string_view get_id() const override { return "REL"; }
};

class ABS final : public Mode
{
public:
    ModeTask execute(CPU &cpu, CPURegister &r) const override;
    std::string_view get_id() const override { return "ABS"; }
};

class ABX final : public Mode
{
public:
    ModeTask execute(CPU &cpu, CPURegister &r) const override;
    std::string_view get_id() const override { return "ABX"; }
};

class ABY final : public Mode
{
public:
    ModeTask execute(CPU &cpu, CPURegister &r) const override;
    std::string_view get_id() const override { return "ABY"; }
};

class IND final : public Mode
{
public:
    ModeTask execute(CPU &cpu, CPURegister &r) const override;
    std::string_view get_id() const override { return "IND"; }
};

class IDX final : public Mode
{
public:
    ModeTask execute(CPU &cpu, CPURegister &r) const override;
    std::string_view get_id() const override { return "IDX"; }
};

class IDY final : public Mode
{
public:
    ModeTask execute(CPU &cpu, CPURegister &r) const override;
    std::string_view get_id() const override { return "IDY"; }
};