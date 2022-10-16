#pragma once

#include <QAction>
#include <QIcon>
#include <QList>
#include <QString>
#include <QWidget>

class Page : public QWidget
{
    Q_OBJECT

public:
    using QWidget::QWidget;
    virtual ~Page(){};

public:
    virtual QIcon icon() const = 0;
    virtual QString title() const = 0;
    virtual QList<QAction *> actions() const
    {
        return {};
    };
};