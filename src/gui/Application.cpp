#include "Application.h"

wxIMPLEMENT_APP_NO_MAIN(Application);

bool Application::OnInit()
{
    MainFrame *frame = new MainFrame("Nesper");
    frame->Show(true);

    return true;
}