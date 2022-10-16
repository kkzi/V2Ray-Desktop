#pragma once

#include "Page.h"
#include "ui_RulesPage.h"

class RulesPage : public Page
{
    Q_OBJECT

public:
    RulesPage(QWidget *parent = nullptr)
        : Page(nullptr)
    {
        ui_.setupUi(this);
    }

public:
    QIcon icon() const override
    {
        return {};
    }

    QString title() const override
    {
        return tr("Rules");
    }

private:
    Ui::RulesPage ui_;
};
