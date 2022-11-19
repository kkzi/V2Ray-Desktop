#include "ClashAppMainWindow.h"
#include "AboutPage.h"
#include "DashboardPage.h"
#include "LogsPage.h"
#include "RulesPage.h"
#include "ServersPage.h"
#include "SettingsPage.h"
#include <QModelIndex>
#include <qitemselectionmodel.h>

ClashAppMainWindow::ClashAppMainWindow()
    : tray_(new QSystemTrayIcon(this))
    , menu_(new QMenu(this))
{
    ui_.setupUi(this);
    ui_.Logo->setPixmap(QPixmap(QString(":/theme/%1/logo.png").arg(ClashApp::theme)).scaledToHeight(48, Qt::SmoothTransformation));
    ui_.Logo->setContentsMargins(10, 10, 6, 10);
    ui_.TitleLayout->setContentsMargins(10, 18, 10, 18);

    addPage(new DashboardPage);
    addPage(new ServersPage);
    addPage(new RulesPage);
    addPage(new SettingsPage);
    addPage(new LogsPage);
    addPage(new AboutPage);

    connect(ui_.Nav->selectionModel(), &QItemSelectionModel::currentRowChanged, this, [this](auto &&index) {
        auto id = index.row();
        auto page = (Page *)ui_.Stack->widget(id);
        ui_.Icon->setPixmap(page->icon().pixmap({ 32, 32 }));
        ui_.Title->setText(page->title());
        ui_.ToolBar->clear();
        ui_.ToolBar->addActions(page->actions());
        ui_.ToolBar->setHidden(page->actions().isEmpty());
        ui_.Stack->setCurrentWidget(page);
    });
    ui_.Nav->setCurrentRow(0);

    connect(tray_, &QSystemTrayIcon::activated, this, [this](auto &&reason) {
        activateWindow();
        showNormal();
    });

    tray_->setIcon(QIcon(QString(":/theme/%1/native_gray.ico").arg(ClashApp::theme)));
    tray_->setContextMenu(menu_);
    tray_->show();
}

void ClashAppMainWindow::addPage(Page *page)
{
    ui_.Nav->addItem(new QListWidgetItem(page->icon(), page->title()));
    ui_.Stack->addWidget(page);
}
