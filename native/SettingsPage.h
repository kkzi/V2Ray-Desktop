#pragma once

#include "ClashApp.h"
#include "Page.h"
#include "libs/json.hpp"
#include "ui_SettingsPage.h"

using namespace nlohmann;

constexpr const char *DEFAULT_GFWLIST_URL = "https://raw.githubusercontent.com/du5/gfwlist/master/Rules/Clash/gfwlist.yml";

class SettingsPage : public Page
{
    Q_OBJECT

public:
    SettingsPage(QWidget *parent = nullptr)
        : Page(nullptr)
    {
        ui_.setupUi(this);

        connect(ui_.LaunchLabel, &ClickableLabel::clicked, ui_.LaunchAtLogin, &QCheckBox::toggle);
        connect(ui_.StartUpLabel, &ClickableLabel::clicked, ui_.HideWindowOnStartUp, &QCheckBox::toggle);
        connect(ui_.Save, &QPushButton::clicked, this, &SettingsPage::updateSettings);
        connect(ClashApp::proxy, &AppProxy::appConfigReady, this, &SettingsPage::init);
        connect(ClashApp::proxy, &AppProxy::appConfigError, this, &SettingsPage::showMessage);

        ClashApp::proxy->getAppConfig();
    }

public:
    QIcon icon() const override
    {
        return QIcon(QString(":/theme/%1/icon-settings.svg").arg(ClashApp::theme));
    }

    QString title() const override
    {
        return tr("Settings");
    }

private:
    void init(const QString &config)
    {
        auto obj = json::parse(config.toStdString());
        ui_.LaunchAtLogin->setChecked(obj.value<bool>("autoStart", false));
        ui_.HideWindowOnStartUp->setChecked(obj.value<bool>("autoStart", false));
        ui_.LanguageBox->setCurrentText(obj.value<std::string>("language", "English").c_str());
        ui_.ProxyMode->setCurrentText(obj.value<std::string>("proxyMode", "Rule").c_str());
        ui_.ListeningIp->setText(obj.value<std::string>("serverIp", "127.0.0.1").c_str());
        ui_.DnsServers->setText(obj.value<std::string>("dns", "8.8.8.8; 4.4.4.4").c_str());
        ui_.SocksPort->setValue(obj.value<int>("socksPort", 1080));
        ui_.HttpPort->setValue(obj.value<int>("httpPort", 1087));
        ui_.GfwUrl->setPlainText(obj.value<std::string>("gfwListUrl", DEFAULT_GFWLIST_URL).c_str());
    }

    void updateSettings()
    {
        ui_.Info->clear();
        json obj{
            { "autoStart", ui_.LaunchAtLogin->isChecked() },
            { "hideWindow", ui_.HideWindowOnStartUp->isChecked() },
            { "defaultSysProxyProtocol", "http" },
            { "dns", ui_.DnsServers->text().toStdString() },
            //{ "gfwListLastUpdated", "Never" },
            { "gfwListUrl", ui_.GfwUrl->toPlainText().toStdString() },
            { "httpPort", ui_.HttpPort->value() },
            { "socksPort", ui_.SocksPort->value() },
            { "language", ui_.LanguageBox->currentText().toStdString() },
            { "proxyMode", ui_.ProxyMode->currentText().toStdString() },
            { "serverIp", ui_.ListeningIp->text().toStdString() },
        };
        ClashApp::proxy->setAppConfig(obj.dump().c_str());
    }

    void showMessage(const QString &msg)
    {
        ui_.Info->setText(msg);
    }

private:
    Ui::SettingsPage ui_;
};
