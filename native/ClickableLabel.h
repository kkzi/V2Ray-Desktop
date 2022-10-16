#pragma once

#include <QLabel>

class ClickableLabel : public QLabel
{
    Q_OBJECT

public:
    using QLabel::QLabel;

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event)
    {
        emit clicked();
    }
};
