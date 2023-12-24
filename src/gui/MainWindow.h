#pragma once

#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/filedlg.h>
#include <wx/wfstream.h>


class Window : public wxFrame
{
public:
    Window(const wxString &title);

    void OnLoad(wxCommandEvent &event);
    void OnQuit(wxCommandEvent &event);

private:
    wxMenuBar *m_menu;
    wxMenu *m_file, *m_options, *m_tools, *m_help;

    wxMenuItem *m_load, *m_quit;
};