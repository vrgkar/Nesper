#pragma once

#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/filedlg.h>
#include <wx/wfstream.h>

#include <sstream>
#include <fstream>

#include "DebugWindow.h"

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title);

    void OnLoadSelect(wxCommandEvent &event);
    void OnDebuggerSelect(wxCommandEvent &event);
    void OnQuitSelect(wxCommandEvent &event);
};