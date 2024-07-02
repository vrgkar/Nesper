#include "DebugWindow.h"

DebugWindow::DebugWindow(wxWindow *parent)
    : wxDialog(parent, wxID_ANY, "Debugger")
{
    tctrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize,
    wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH);

    wxButton *button_next = new wxButton(this, wxID_ANY, "Next");

    button_next->Bind(wxEVT_BUTTON, &DebugWindow::OnButtonDown, this);

    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    vbox->Add(tctrl, 3, wxEXPAND | wxALL, 5);
    vbox->Add(button_next, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);

    SetSizer(vbox);
}

void DebugWindow::OnButtonDown(wxCommandEvent & WXUNUSED(event))
{
    NES::step_next_instruction();
    tctrl->AppendText(NES::get_current_instr_disassembly());
    tctrl->AppendText("\n");
}