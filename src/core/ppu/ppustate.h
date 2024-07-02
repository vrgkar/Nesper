#pragma once

#include "common/pch.h"

#include "ppuregister.h"

struct PPUState
{
    PPURegister r;

    bool nmi_occured = false;
    bool nmi_output = false;

    int scanline = 261;
};