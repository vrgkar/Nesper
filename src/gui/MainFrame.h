#pragma once

#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/filedlg.h>
#include <wx/wfstream.h>
#include <wx/dcclient.h>
#include <wx/graphics.h>

#include <sstream>
#include <fstream>

#include "DebugWindow.h"
#include "GLCanvas.h"

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title);

    void OnLoadSelect       (wxCommandEvent &event);
    void OnDebuggerSelect   (wxCommandEvent &event);
    void OnQuitSelect       (wxCommandEvent &event);
    void OnPaint            (wxPaintEvent   &event);
    void OnTimer            (wxTimerEvent   &event);

private:
    GLCanvas *m_canvas;
    wxBitmap m_bitmap;
    wxPanel *m_panel;
    wxTimer *m_timer;

    bool m_rom_loaded = false;
};