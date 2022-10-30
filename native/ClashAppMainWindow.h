#pragma once

#include "Page.h"
#include "ui_ClashAppMainWindow.h"
#include <QWidget>
#include <qsystemtrayicon.h>
#include <qmenu.h>

class ClashAppMainWindow : public QWidget
{
    Q_OBJECT

public:
    ClashAppMainWindow();

private:
    void addPage(Page *page);

private:
    Ui::ClashAppMainWindow ui_;
    QSystemTrayIcon* tray_{};
    QMenu* menu_{};
};
