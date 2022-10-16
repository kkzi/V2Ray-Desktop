#include "ClashApp.h"
#include "ClashAppMainWindow.h"

int main(int argc, char **argv)
{
    auto a = ClashApp::init(argc, argv);
    q_check_ptr(a);

    ClashApp::window->show();

    return a->exec();
}