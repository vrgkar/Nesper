#pragma once

#include <wx/wx.h>

#include "MainFrame.h"

class Application : public wxApp
{
public:
    virtual bool OnInit() override;
};

wxDECLARE_APP(Application);