#pragma once

#include "ClashApp.h"
#include "libs/json.hpp"
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

using json = nlohmann::json;

class SubscriptionNewWidget : public QWidget
{
    Q_OBJECT
public:
    SubscriptionNewWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        auto form = new QFormLayout;
        form->addRow(tr("Name"), nameEdit_ = new QLineEdit);
        form->addRow(tr("Subscription URL"), urlEdit_ = new QLineEdit);

        auto okbtn = new QPushButton(tr("Confirm"), this);
        auto oprow = new QHBoxLayout;
        oprow->addWidget(okbtn);
        oprow->addWidget(info_ = new QLabel, 1);

        auto layout = new QVBoxLayout(this);
        layout->addLayout(form);
        layout->addLayout(oprow);
        layout->addStretch(1);

        connect(okbtn, &QPushButton::clicked, this, &SubscriptionNewWidget::createNewSubscription);
        connect(ClashApp::proxy, &AppProxy::serverConfigError, this, &SubscriptionNewWidget::showInfo);
        connect(ClashApp::proxy, &AppProxy::serverChanged, this, &SubscriptionNewWidget::onServerChanged);
        connect(ClashApp::proxy, &AppProxy::serversChanged, this, [] {
            ClashApp::proxy->getServers();
        });
    }

private:
    void createNewSubscription()
    {
        info_->clear();
        auto name = nameEdit_->text();
        auto url = urlEdit_->text();
        ClashApp::proxy->addSubscriptionUrl(url);
    }

    void onServersReady(const QString &config)
    {
    }

    void onServerChanged(QString name, QString config)
    {
    }

    void onServersChanged()
    {
        ClashApp::proxy->getServers();
    }

    void showInfo(const QString &msg)
    {
        info_->setText(msg);
    }

private:
    QLineEdit *nameEdit_;
    QLineEdit *urlEdit_;
    QLabel *info_;
};
