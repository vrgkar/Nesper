#pragma once

#include <wx/wx.h>
#include "window/window.h"

class App : public wxApp
{
public:
    virtual bool OnInit();
};

DECLARE_APP(App)