#pragma once

#include <wx/wx.h>
#include <wx/stc/stc.h>
#include <wx/richtext/richtextctrl.h>
#include "../core/nes.h"

class DebugWindow : public wxDialog
{
public:
    DebugWindow(wxWindow *parent);

    void OnButtonDown(wxCommandEvent &event);

private:
    wxRichTextCtrl *richTextCtrl;
};