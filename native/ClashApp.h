#pragma once

#include <QApplication>

class ClashAppMainWindow;
class AppProxy;

struct ClashApp
{
    static QApplication *app;
    static AppProxy *proxy;
    static ClashAppMainWindow *window;

    static QApplication *init(int argc, char **argv);
};
