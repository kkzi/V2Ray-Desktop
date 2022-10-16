#pragma once

#include "Page.h"

class LogsPage : public Page
{
    Q_OBJECT

public:
    LogsPage(QWidget *parent = nullptr)
        : Page(nullptr)
    {
    }

public:
    QIcon icon() const override
    {
        return {};
    }

    QString title() const override
    {
        return tr("Logs");
    }

private:
};
