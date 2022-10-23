#pragma once

#include "Page.h"
#include "ServerItem.h"
#include "ServersModel.h"
#include "SimpleTableView.h"
#include "SubscriptionNewWidget.h"
#include <QActionGroup>
#include <QLabel>
#include <QListView>
#include <QListWidget>
#include <QStackedLayout>
#include <QTableView>
#include <QToolBar>
#include <QTreeView>

class ServersPage : public Page
{
    Q_OBJECT

public:
    ServersPage(QWidget *parent = nullptr)
        : Page(parent)
        , subsbar_(new QToolBar(this))
        , group_(this)
        , stack_(new QStackedLayout)
        , serverTable_(new SimpleTableView(this))
        , model_(new ServersModel)
    {
        setObjectName("ServersPage");
        serverTable_->setModel(model_);
        for (auto i = 0; i < model_->columnCount(); ++i)
        {
            serverTable_->setColumnWidth(i, model_->width(i));
        }

        auto creation = subsbar_->addAction(tr("New Subscription"));
        creation->setData("creation");
        creation->setCheckable(true);
        group_.addAction(creation);

        stack_->addWidget(new SubscriptionNewWidget(this));
        stack_->addWidget(serverTable_);

        auto layout = new QVBoxLayout(this);
        layout->addWidget(subsbar_, 0);
        layout->addLayout(stack_, 1);

        connect(subsbar_, &QToolBar::actionTriggered, this, &ServersPage::showOrCreateSubscription);
        connect(serverTable_, &QTableView::doubleClicked, this, &ServersPage::toggleServer);
        connect(ClashApp::proxy, &AppProxy::serversReady, this, &ServersPage::updateServers);
        connect(ClashApp::proxy, &AppProxy::serverConnectivityChanged, this, &ServersPage::updateServerStatus);
        ClashApp::proxy->getServers();
    }

public:
    QIcon icon() const override
    {
        return QIcon(":/images/icon-servers.svg");
    }

    QString title() const override
    {
        return tr("Servers");
    }

    QList<QAction *> actions() const
    {
        return {};
    }

private:
    void showOrCreateSubscription(const QAction *action)
    {
        auto id = action->data().toString();
        bool isCreation = id == "creation";
        stack_->setCurrentIndex(isCreation ? 0 : 1);
    }

    void toggleServer(const QModelIndex &index)
    {
        auto server = model_->item(index);
        if (!server)
        {
            return;
        }
        auto isConnected = server->status == ServerItem::Status::Connected;
        ClashApp::proxy->setServerConnection(server->name, !isConnected);
    }

    void updateServers(const QString &text)
    {
        QList<ServerItem> servers;
        QSet<QString> subs;
        auto arr = json::parse(text.toStdString());
        for (auto &&it : arr)
        {
            ServerItem si(it);
            servers << si;
            subs << si.subscription;
        }
        if (servers.isEmpty())
        {
            subsbar_->actions().first()->trigger();
            return;
        }

        auto acts = subsbar_->actions();
        for (auto i = 1; i < acts.size(); ++i)
        {
            auto act = acts.at(i);
            subsbar_->removeAction(act);
        }
        auto sarr = subs.values();
        for (auto i = 0; i < subs.size(); ++i)
        {
            auto act = subsbar_->addAction(tr("Subscription %1").arg(i));
            act->setData(sarr.value(i));
            act->setCheckable(true);
            group_.addAction(act);
        }
        model_->setupData(std::move(servers));
        stack_->setCurrentIndex(1);
        // subsbar_->actions().first()->trigger();
    }

    void updateServerStatus(const QString &name, bool isConnecetd)
    {
        model_->updateItemStatus(name, isConnecetd);
    }

private:
    QToolBar *subsbar_{ nullptr };
    QActionGroup group_;
    QStackedLayout *stack_{ nullptr };
    SimpleTableView *serverTable_{ nullptr };
    ServersModel *model_{ nullptr };
};
