#pragma once

#include "Page.h"
#include "ui_ClashAppMainWindow.h"
#include <QWidget>

class ClashAppMainWindow : public QWidget
{
    Q_OBJECT

public:
    ClashAppMainWindow();

private:
    void addPage(Page *page);

private:
    Ui::ClashAppMainWindow ui_;
};
