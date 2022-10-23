#include "ClashApp.h"
#include "ClashAppMainWindow.h"
#include "configurator.h"
#include <QDateTimeEdit>
#include <QFile>
#include <QTextStream>

static void MessageHandler(QtMsgType msgType, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);
    static QString lastMsg;
    if (lastMsg == msg)
    {
        return;
    }
    lastMsg = msg;

    QString dt = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString logMessage("%1 [%2] %3");

    static QMap<QtMsgType, QChar> type2str{
        { QtDebugMsg, 'D' },
        { QtInfoMsg, 'I' },
        { QtWarningMsg, 'W' },
        { QtCriticalMsg, 'E' },
        { QtFatalMsg, 'E' },
    };

    auto line = logMessage.arg(dt).arg(type2str.value(msgType, 'D')).arg(msg);
    QTextStream(stdout) << line << Qt::endl;
    if (msgType != QtDebugMsg)
    {
        ClashApp::appendLog(line);
    }
}

int WinMain(int argc, char **argv)
{
    auto a = ClashApp::init(argc, argv);
    a->setStyleSheet("file:///:/native.css");
    q_check_ptr(a);

    qInstallMessageHandler(MessageHandler);

    ClashApp::window->show();

    return a->exec();
}