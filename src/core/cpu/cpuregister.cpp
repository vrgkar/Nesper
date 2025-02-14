#include "cpuregister.h"

CPURegister::CPURegister()
    : sr{0x24u}, a(0x0u), x(0x0u), y(0x0u), sp(0xFDu)
{}

CPURegister::~CPURegister()
{}

void CPURegister::set(StackRegister f, bool b)
{
    sr = (sr & ~(1u << static_cast<unsigned>(f))) | (b << static_cast<unsigned>(f));
}

bool CPURegister::test(StackRegister f)
{
    return sr & (1u << static_cast<unsigned>(f));
}

void CPURegister::reset()
{
    sp -= 3u;
    set(StackRegister::I, true);
}
