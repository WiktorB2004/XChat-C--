#ifndef CUSTOM_ELEMENTS_H
#define CUSTOM_ELEMENTS_H

#include <QLabel>
#include <QMouseEvent>
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <iostream>
#include <atomic>
#include "app_windows.h"
#include "app_windows.h"

class ClickableLabel : public QLabel
{
    Q_OBJECT
signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            emit clicked();
        }
        QLabel::mousePressEvent(event);
    }
};

#endif // CUSTOM_ELEMENTS_H