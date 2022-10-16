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
{
    ui_.setupUi(this);

    addPage(new DashboardPage);
    addPage(new ServersPage);
    addPage(new RulesPage);
    addPage(new SettingsPage);
    addPage(new LogsPage);
    addPage(new AboutPage);

    connect(ui_.Nav->selectionModel(), &QItemSelectionModel::currentRowChanged, this, [this](auto &&index) {
        auto id = index.row();
        auto page = (Page *)ui_.Stack->widget(id);
        ui_.Icon->setPixmap(page->icon().pixmap({ 48, 48 }));
        ui_.Title->setText(page->title());
        ui_.ToolBar->clear();
        ui_.ToolBar->addActions(page->actions());
        ui_.ToolBar->setHidden(page->actions().isEmpty());
        ui_.Stack->setCurrentWidget(page);
    });
    ui_.Nav->setCurrentRow(0);
}

void ClashAppMainWindow::addPage(Page *page)
{
    Q_CHECK_PTR(page);

    ui_.Nav->addItem(new QListWidgetItem(page->icon(), page->title()));
    ui_.Stack->addWidget(page);
}
