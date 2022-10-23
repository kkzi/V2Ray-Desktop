#pragma once

#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

class StatusItemPane : public QFrame
{
    Q_OBJECT

public:
    StatusItemPane(QString title, QWidget *content = nullptr, QWidget *parent = nullptr)
        : QFrame(parent)
        , title_(new QLabel)
    {
        title_->setObjectName("Title");
        title_->setText(std::move(title));

        detail_ = content ? content : new QWidget;
        detail_->setObjectName("Detail");

        auto layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        layout->addWidget(title_);
        layout->addWidget(detail_, 1);
    }

private:
    QLabel *title_{ nullptr };
    QWidget *detail_{ nullptr };
};
