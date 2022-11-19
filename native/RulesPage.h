#pragma once

#include "ClashApp.h"
#include "Page.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class RulesPage : public Page
{
    Q_OBJECT

public:
    RulesPage(QWidget *parent = nullptr)
        : Page(parent)
        , info_(new QLabel)
    {
        setObjectName("RulesPage");

        auto updateBtn = new QPushButton(tr("Update GFW List"));
        auto row0 = new QHBoxLayout;
        row0->addWidget(updateBtn);
        row0->addStretch(1);

        auto row1 = new QHBoxLayout;
        row1->setSpacing(0);
        row1->addWidget(new QLabel(tr("GFW List Last Updated on ")));
        row1->addWidget(info_, 1);

        auto layout = new QVBoxLayout(this);
        layout->addLayout(row0);
        layout->addLayout(row1);
        layout->addStretch(1);

        connect(updateBtn, &QPushButton::clicked, this, [updateBtn] {
            updateBtn->setDisabled(true);
            ClashApp::proxy->updateGfwList();
        });
        connect(ClashApp::proxy, &AppProxy::gfwListUpdated, this, [this, updateBtn](auto &&txt) {
            info_->setText(txt);
            updateBtn->setEnabled(true);
        });
        connect(ClashApp::proxy, &AppProxy::appConfigReady, this, &RulesPage::onConfigReady);
    }

public:
    QIcon icon() const override
    {
        return QIcon(QString(":/theme/%1/icon-rules.svg").arg(ClashApp::theme));
    }

    QString title() const override
    {
        return tr("Rules");
    }

private:
    void onConfigReady(const QString &config)
    {
        auto obj = json::parse(config.toStdString());
        QString text = obj.value<std::string>("gfwListLastUpdated", "").c_str();
        info_->setText(text.isEmpty() ? tr("Never updated") : text);
    }

private:
    QLabel *info_{ nullptr };
};
