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

class ChatInput : public QWidget
{
    Q_OBJECT

private:
    QLineEdit *inputLineEdit;

public:
    ChatWindow *window;
    ChatInput(ChatWindow *window, QWidget *parent = nullptr) : window(window), QWidget(parent)
    {
        // Initialize inputLineEdit
        inputLineEdit = new QLineEdit(this);

        // Create a QWidget to hold the grid
        QWidget *gridWidget = new QWidget(this); // Note: Use "this" as the parent
        QGridLayout *gridLayout = new QGridLayout(gridWidget);

        QLabel *gridLabel = new QLabel(QString("(0, 0)"));
        gridLabel->setAlignment(Qt::AlignCenter);
        gridLabel->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 128); color: white; }"); // Set transparent background
        gridLayout->addWidget(gridLabel, 0, 0);

        ClickableLabel *submitLabel = new ClickableLabel;
        QPixmap submitImage("assets/images/submit.png");
        submitLabel->setPixmap(submitImage.scaled(QSize{25, 25}, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));

        // Create a horizontal layout for the form
        QHBoxLayout *formLayout = new QHBoxLayout;
        formLayout->addWidget(inputLineEdit);
        formLayout->addWidget(submitLabel);

        QWidget *inputWidget = new QWidget();
        inputWidget->setLayout(formLayout);
        gridLayout->addWidget(inputWidget, 1, 0);

        // Set layout for the gridWidget
        gridWidget->setLayout(gridLayout);
    }
};

#endif // CUSTOM_ELEMENTS_H