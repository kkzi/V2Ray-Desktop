#pragma once

#include <QTableView>
#include <QScrollBar>
#include <qheaderview.h>
#include <qitemselectionmodel.h>

class SimpleTableView : public QTableView
{
    Q_OBJECT

public:
    SimpleTableView(QWidget *parent = nullptr)
        : QTableView(parent)
    {
        setShowGrid(false);
        setSelectionMode(QTableView::SingleSelection);
        setSelectionBehavior(QTableView::SelectRows);
        setEditTriggers(QTableView::NoEditTriggers);
        setFocusPolicy(Qt::NoFocus);
        setAlternatingRowColors(false);

        auto hheader = horizontalHeader();
        hheader->setObjectName("HorizontalHeader");
        hheader->setDefaultAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        hheader->setHighlightSections(false);
        hheader->setStretchLastSection(true);

        auto vheader = verticalHeader();
        vheader->setObjectName("VerticalHeader");
        vheader->setDefaultAlignment(Qt::AlignVCenter | Qt::AlignRight);
        vheader->hide();

        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
};
