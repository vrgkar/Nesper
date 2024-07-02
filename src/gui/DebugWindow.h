#pragma once

#include <wx/wx.h>
#include <wx/stc/stc.h>
#include <wx/textctrl.h>
#include "../core/nes.h"

class DebugWindow : public wxDialog
{
public:
    DebugWindow(wxWindow *parent);

    void OnButtonDown(wxCommandEvent &event);

private:
    wxTextCtrl *tctrl;
};