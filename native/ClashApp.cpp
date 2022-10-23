#include "ClashApp.h"
#include "ClashAppMainWindow.h"
#include "configurator.h"
#include <QStringList>
#include <appproxy.h>

QApplication *ClashApp::app = nullptr;
AppProxy *ClashApp::proxy = nullptr;
ClashAppMainWindow *ClashApp::window = nullptr;

static QTextStream *logstream_;

QApplication *ClashApp::init(int argc, char **argv)
{
    auto a = new QApplication(argc, argv);
    ClashApp::app = a;
    ClashApp::proxy = new AppProxy(a);
    ClashApp::window = new ClashAppMainWindow;

    {
        static QFile logFile(Configurator::getAppLogFilePath());
        logFile.open(QIODevice::ReadWrite | QIODevice::Truncate);
        logstream_ = new QTextStream(&logFile);
    }

    return ClashApp::app;
}

void ClashApp::destroy()
{
    logstream_->device()->close();
    delete logstream_;
}

static QStringList logs_;
void ClashApp::appendLog(const QString &line)
{
    logs_ << line;
    *logstream_ << line << Qt::endl;
}

QStringList ClashApp::recentLogs()
{
    QStringList arr;
    arr.swap(logs_);
    return arr;
}
