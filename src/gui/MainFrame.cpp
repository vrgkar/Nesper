#include "MainFrame.h"

MainFrame::MainFrame(const wxString &title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600))
{
    auto m_menubar = new wxMenuBar;

    auto m_file_menu = new wxMenu;
    auto m_options_menu = new wxMenu;
    auto m_tools_menu = new wxMenu;
    auto m_help_menu = new wxMenu;

    auto m_load_option = new wxMenuItem(m_file_menu, wxID_ANY, wxT("&Load\tCtrl+A"));
    auto m_quit_option = new wxMenuItem(m_file_menu, wxID_ANY, wxT("&Quit\tCtrl+Q"));
    auto m_debugger_option = new wxMenuItem(m_file_menu, wxID_ANY, wxT("&Debugger"));

    m_file_menu->Append(m_load_option);
    m_file_menu->Append(m_quit_option);
    m_tools_menu->Append(m_debugger_option);

    m_menubar->Append(m_file_menu, wxT("&File"));
    m_menubar->Append(m_options_menu, wxT("&Options"));
    m_menubar->Append(m_tools_menu, wxT("&Tools"));
    m_menubar->Append(m_help_menu, wxT("&Help"));

    m_menubar->Bind(wxEVT_MENU, &MainFrame::OnLoadSelect, this, m_load_option->GetId());
    m_menubar->Bind(wxEVT_MENU, &MainFrame::OnDebuggerSelect, this, m_debugger_option->GetId());
    m_menubar->Bind(wxEVT_MENU, &MainFrame::OnQuitSelect, this, m_quit_option->GetId());

    SetMenuBar(m_menubar);
    Centre();
}

void MainFrame::OnQuitSelect(wxCommandEvent & WXUNUSED(event))
{
    Close(true);
}

void MainFrame::OnLoadSelect(wxCommandEvent & WXUNUSED(event))
{
    wxFileDialog openFileDialog(this, _("Open NES file"), "", "", "NES files (*.nes)|*.nes", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    std::ifstream t(openFileDialog.GetPath());
    std::stringstream buffer;
    buffer << t.rdbuf();
    
    NES::load_rom(buffer.str());

}

void MainFrame::OnDebuggerSelect(wxCommandEvent & WXUNUSED(event))
{
    static DebugWindow *debug_window = nullptr;

    if (!debug_window)
    {
        debug_window = new DebugWindow(this);
        debug_window->Bind(wxEVT_CLOSE_WINDOW, [](wxCloseEvent &event) {
            debug_window->Destroy();
            debug_window = nullptr;
            event.Skip();
         });
         debug_window->Show();
    }

}