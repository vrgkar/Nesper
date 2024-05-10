#include "MainFrame.h"

MainFrame::MainFrame(const wxString &title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600))
{
    m_menu = new wxMenuBar;

    m_file = new wxMenu;
    m_options = new wxMenu;
    m_tools = new wxMenu;
    m_help = new wxMenu;

    m_load = new wxMenuItem(m_file, wxID_OPEN, wxT("&Load\tCtrl+A"));
    m_quit = new wxMenuItem(m_file, wxID_EXIT, wxT("&Quit\tCtrl+Q"));

    m_file->Append(m_load);
    m_file->Append(m_quit);

    Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnQuit));
    Connect(wxID_OPEN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnLoad));

    m_menu->Append(m_file, wxT("&File"));
    m_menu->Append(m_options, wxT("&Options"));
    m_menu->Append(m_tools, wxT("&Tools"));
    m_menu->Append(m_help, wxT("&Help"));

    SetMenuBar(m_menu);
    Centre();
}

void MainFrame::OnQuit(wxCommandEvent & WXUNUSED(event))
{
    Close(true);
}

void MainFrame::OnLoad(wxCommandEvent & WXUNUSED(event))
{
    wxFileDialog openFileDialog(this, _("Open NES file"), "", "", "NES files (*.nes)|*.nes", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;    
}

