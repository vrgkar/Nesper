#pragma once

#include "pch.h"

struct Task {
    // The coroutine level type
    struct promise_type {
        using Handle = std::coroutine_handle<promise_type>;
        void return_value(int value) { current_value = value; }
        Task get_return_object() {
            return Task{Handle::from_promise(*this)};
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
    explicit Task(promise_type::Handle coro) : coro_(coro) {}
    ~Task() {
        if (coro_) coro_.destroy();
    }
    // Make move-only
    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;
    Task(Task&& t) noexcept : coro_(t.coro_) { 
        t.coro_ = {};
    }
    Task& operator=(Task&& t) noexcept {
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