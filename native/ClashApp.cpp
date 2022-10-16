#include "ClashApp.h"
#include "ClashAppMainWindow.h"
#include <appproxy.h>

QApplication *ClashApp::app = nullptr;
AppProxy *ClashApp::proxy = nullptr;
ClashAppMainWindow *ClashApp::window = nullptr;

QApplication *ClashApp::init(int argc, char **argv)
{
    auto a = new QApplication(argc, argv);
    ClashApp::app = a;
    ClashApp::proxy = new AppProxy(a);
    ClashApp::window = new ClashAppMainWindow;

    //auto worker = new QThread(a);
    //ClashApp::proxy->moveToThread(worker);
    //worker->start();

    return ClashApp::app;
}
