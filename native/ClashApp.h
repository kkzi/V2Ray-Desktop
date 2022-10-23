#pragma once

#include <QApplication>
#include <QFile>

class ClashAppMainWindow;
class AppProxy;

struct ClashApp
{
    static QApplication *app;
    static AppProxy *proxy;
    static ClashAppMainWindow *window;

    static QApplication *init(int argc, char **argv);
    static void destroy();

    static void appendLog(const QString &line);
    static QStringList recentLogs();
};
