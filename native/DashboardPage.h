#pragma once

#include "ClashApp.h"
#include "Page.h"
#include "StatusItemPane.h"
#include "libs/json.hpp"
#include <QSysInfo>
#include <QTimer>
#include <QVBoxLayout>
#include <appproxy.h>

using json = nlohmann::json;

class DashboardPage : public Page
{
    Q_OBJECT

public:
    DashboardPage(QWidget *parent = nullptr)
        : Page(parent)
    {
        auto layout = new QVBoxLayout(this);
        layout->setSpacing(15);
        layout->addWidget(new StatusItemPane(tr("Network Status"), network_ = new QLabel));
        layout->addWidget(new StatusItemPane(tr("Proxy Settings"), proxy_ = new QLabel));
        layout->addWidget(new StatusItemPane(tr("Operation System"), new QLabel(QSysInfo::prettyProductName())));
        layout->addWidget(new StatusItemPane(tr("App Version"), new QLabel(ClashApp::app->applicationVersion())));
        layout->addWidget(new StatusItemPane(tr("Clash Version"), clashVersion_ = new QLabel));
        layout->addStretch(1);

        connect(ClashApp::proxy, &AppProxy::networkStatusReady, this, &DashboardPage::updateNetworkStatus);
        connect(ClashApp::proxy, &AppProxy::proxySettingsReady, this, &DashboardPage::updateProxyStatus);
        connect(ClashApp::proxy, &AppProxy::v2RayCoreVersionReady, this, &DashboardPage::updateCoreVersion);

        connect(&timer_, &QTimer::timeout, this, &DashboardPage::getLatestStatus);
        timer_.setSingleShot(true);
        timer_.setInterval(5000);
    }

public:
    QIcon icon() const override
    {
        return QIcon(":/images/icon-dashboard.svg");
    }

    QString title() const override
    {
        return tr("Dashboard");
    }

protected:
    void showEvent(QShowEvent *event) override
    {
        getLatestStatus();
    }

    void hideEvent(QHideEvent *event) override
    {
        timer_.stop();
        return Page::hideEvent(event);
    }

private:
    void getLatestStatus()
    {
        ClashApp::proxy->getNetworkStatus();
        ClashApp::proxy->getProxySettings();
        ClashApp::proxy->getV2RayCoreVersion();

        timer_.start();
    }

    void updateNetworkStatus(const QString &status)
    {
        auto obj = json::parse(status.toStdString());
        if (obj.value<bool>("isGoogleAccessible", false))
        {
            network_->setText(tr("Everything works fine.\nYou can access the free Internet."));
        }
        else if (obj.value<bool>("isBaiduAccessible", false))
        {
            network_->setText(tr("Please check your proxy settings."));
        }
        else
        {
            network_->setText(tr("You're offline.\nPlease check the network connection."));
        }
    }

    void updateProxyStatus(const QString &status)
    {
        auto obj = json::parse(status.toStdString());
        auto text = tr("System Proxy\t%1\nProxy Mode\t%2\nClash Core\t%3")
                        .arg(obj.value<std::string>("systemProxy", "").c_str())
                        .arg(obj.value<std::string>("proxyMode", "").c_str())
                        .arg(obj.value<bool>("isV2RayRunning", false) ? tr("Running\nConnected Servers: %1").arg([arr = obj.at("connectedServers")] {
                            QStringList lines;
                            std::transform(arr.begin(), arr.end(), std::back_inserter(lines), [](auto &&it) {
                                return QString("- %1").arg("");
                            });
                            return lines.isEmpty() ? tr("Please connect to at least one server!") : lines.join("\n");
                        }())
                                                                      : tr("Not Running"));
        proxy_->setText(text);
    }

    void updateCoreVersion(const QString &status)
    {
        clashVersion_->setText(status);
    }

private:
    QTimer timer_;
    int timerId_{ 0 };
    QLabel *network_{ nullptr };
    QLabel *proxy_{ nullptr };
    QLabel *clashVersion_{ nullptr };
};
