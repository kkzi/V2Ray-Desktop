#pragma once

#include "ClashApp.h"
#include "FileDownloader.h"
#include "Page.h"
#include "appproxy.h"
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

class AboutPage : public Page
{
    Q_OBJECT

public:
    AboutPage(QWidget *parent = nullptr)
        : Page(parent)
        , appVer_(new QLabel)
        , coreVer_(new QLabel)
    {
        auto checkAppUpdateBtn = new QPushButton(tr("Check for Updates"));
        auto checkCoreUpdateBtn = new QPushButton(tr("Check for Updates"));

        appVer_->setText(ClashApp::app->applicationVersion());

        auto row0 = new QHBoxLayout;
        row0->addWidget(appVer_);
        row0->addWidget(checkAppUpdateBtn);
        row0->addStretch();

        auto row1 = new QHBoxLayout;
        row1->addWidget(coreVer_);
        row1->addWidget(checkCoreUpdateBtn);
        row1->addStretch();

        auto layout = new QFormLayout(this);
        layout->addRow(tr("Clash Native Version"), row0);
        layout->addRow(tr("Clash Core Version"), row1);
        layout->addRow(tr("Project Page"), new QLabel("https://github.com/kkzi/V2Ray-Desktop"));
        layout->addRow(tr("Thanks"), new QLabel("todo"));

        connect(checkAppUpdateBtn, &QPushButton::clicked, this, &AboutPage::checkAppUpdate);
        connect(checkCoreUpdateBtn, &QPushButton::clicked, this, &AboutPage::checkCoreUpdateOrDownload);
        connect(ClashApp::proxy, &AppProxy::v2RayCoreVersionReady, this, [this, btn = checkCoreUpdateBtn](auto &&ver) {
            coreVer_->setText(ver);
            btn->setText(ver == tr("Not Installed") ? tr("Download") : tr("Check for Updates"));
        });
        connect(ClashApp::proxy, &AppProxy::latestReleaseReady, this, &AboutPage::showRelease);
        connect(ClashApp::proxy, &AppProxy::latestReleaseError, this, &AboutPage::showError);

        ClashApp::proxy->getV2RayCoreVersion();
    }

public:
    QIcon icon() const override
    {
        return QIcon(":/images/icon-about.svg");
    }

    QString title() const override
    {
        return tr("About");
    }

private:
    void checkAppUpdate()
    {
    }

    void checkCoreUpdateOrDownload()
    {
        downloader_.downloadCore();
        //ClashApp::proxy->getLatestRelease("v2ray-core");
    }

    void showRelease(const QString &name, const QString &detail)
    {
    }

    void showError(const QString &msg)
    {
    }

private:
    QLabel *appVer_{ nullptr };
    QLabel *coreVer_{ nullptr };
    FileDownloader downloader_;
};
