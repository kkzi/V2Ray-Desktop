#pragma once

#include "Page.h"

class ServersPage : public Page
{
    Q_OBJECT

public:
    ServersPage(QWidget *parent = nullptr)
        : Page(nullptr)
        , addSub_(new QAction(tr("Subcriptions")))
    {
    }

public:
    QIcon icon() const override
    {
        return {};
    }

    QString title() const override
    {
        return tr("Servers");
    }

    QList<QAction *> actions() const
    {
        return { addSub_ };
    }

private:
    QAction *addSub_;
};
