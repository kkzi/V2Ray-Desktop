#pragma once

#include "ClashApp.h"
#include "Page.h"
#include <QPlainTextEdit>
#include <QScrollBar>

class LogsPage : public Page
{
    Q_OBJECT

public:
    LogsPage(QWidget *parent = nullptr)
        : Page(nullptr)
    {
        setObjectName("LogsPage");
        auto layout = new QVBoxLayout(this);
        layout->addWidget(edit_ = new QPlainTextEdit(this));
        edit_->setReadOnly(true);
        edit_->setMaximumBlockCount(1000);

        startTimer(500);
    }

public:
    QIcon icon() const override
    {
        return QIcon(":/images/icon-logs.svg");
    }

    QString title() const override
    {
        return tr("Logs");
    }

protected:
    void timerEvent(QTimerEvent *event) override
    {
        for (auto &&it : ClashApp::recentLogs())
        {
            edit_->appendPlainText(it);
        }
        auto bar = edit_->verticalScrollBar();
        bar->setValue(bar->maximum());
        edit_->ensureCursorVisible();
    }

private:
    QPlainTextEdit *edit_;
};
