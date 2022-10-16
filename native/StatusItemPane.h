#pragma once

#include "ui_StatusItemPane.h"
#include <QWidget>

class StatusItemPane : public QWidget
{
    Q_OBJECT

public:
    StatusItemPane(QString title, QWidget *content = nullptr, QWidget *parent = nullptr)
        : QWidget(parent)
    {
        ui_.setupUi(this);
        ui_.Title->setText(std::move(title));
        if (content)
        {
            layout()->replaceWidget(ui_.Detail, content);
            content->setObjectName("Detail");
            ui_.Detail->deleteLater();
            ui_.Detail = content;
        }
    }

private:
    Ui::StatusItemPane ui_;
};
