#include "app.h"

IMPLEMENT_APP_NO_MAIN(App)

bool App::OnInit()
{
    Window *window = new Window(wxT("Nesper"));
    window->Show(true);

    return true;
}