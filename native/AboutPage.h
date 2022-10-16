#pragma once

#include "Page.h"
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
        auto checkAppUpdateBtn = new QPushButton(tr("Check for updates"));
        auto checkCoreUpdateBtn = new QPushButton(tr("Check for updates"));

        auto row0 = new QHBoxLayout;
        row0->addWidget(appVer_);
        row0->addWidget(checkAppUpdateBtn);
        row0->addStretch();

        auto row1 = new QHBoxLayout;
        row1->addWidget(coreVer_);
        row1->addWidget(checkCoreUpdateBtn);
        row1->addStretch();

        auto layout = new QFormLayout(this);
        layout->addRow(tr("Clash Desktop Version"), row0);
        layout->addRow(tr("Clash Version"), row1);
        layout->addRow(tr("Project Page"), new QLabel("https://github.com/kkzi/V2Ray-Desktop"));
        layout->addRow(tr("Thanks"), new QLabel("todo"));

        connect(checkAppUpdateBtn, &QPushButton::clicked, this, &AboutPage::checkAppUpdate);
        connect(checkCoreUpdateBtn, &QPushButton::clicked, this, &AboutPage::checkCoreUpdate);
    }

public:
    QIcon icon() const override
    {
        return {};
    }

    QString title() const override
    {
        return tr("About");
    }

private:
    void checkAppUpdate()
    {
    }

    void checkCoreUpdate()
    {
    }

private:
    QLabel *appVer_{ nullptr };
    QLabel *coreVer_{ nullptr };
};
