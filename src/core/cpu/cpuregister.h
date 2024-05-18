#pragma once

#include <cstdint>
#include <bitset>

struct CPURegister
{
    enum class StackRegister
    {
        C,
        Z,
        I,
        D,
        B,
        X,
        V,
        N
    };

    CPURegister();
    ~CPURegister();

    void reset();

    void set(StackRegister f, bool b);
    bool test(StackRegister f);

    /* Accumulator Register */
    uint8_t a;

    /* Index Registers */
    uint8_t x;
    uint8_t y;

    /* Program Counter */
    uint16_t pc;

    /* Stack Pointer */
    uint8_t sp;

    /* Stack Register */
    uint8_t sr;

    /* Memory Address Register */
    uint16_t mar;

    /* Memory Data Register */
    uint8_t mdr;

};