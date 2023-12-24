#pragma once

#include <wx/wx.h>

#include "MainWindow/MainWindow.h"

class App : public wxApp
{
public:
    virtual bool OnInit();
};

DECLARE_APP(App)