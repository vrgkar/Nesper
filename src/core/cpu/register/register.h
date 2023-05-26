#pragma once

#include <cstdint>
#include <bitset>

struct Register
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

    Register();
    ~Register();

    void reset();

    void set(StackRegister f, bool b);
    bool test(StackRegister f);

    /* Accumulator Register */
    uint8_t a;

    /* Index Registers */
    uint8_t x, y;

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