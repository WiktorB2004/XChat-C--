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
#include "client.h"
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
public:
    std::atomic<Client *> &curr_client;
    ChatWindow *window;
    ChatInput(ChatWindow *window, std::atomic<Client *> &client, QWidget *parent = nullptr) : QWidget(parent), window(window), curr_client(client)
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

        // Connect the clicked signal of submitLabel to handleSendMessage slot
        QObject::connect(submitLabel, &ClickableLabel::clicked, this, &ChatInput::storeInput);
        QObject::connect(submitLabel, &ClickableLabel::clicked, this, &ChatInput::handleMessageSend);
    }

private slots:
    void storeInput()
    {
        QString inputText = inputLineEdit->text();
        Client *tmp_client = curr_client.load();
        tmp_client->message = inputText.toStdString();
        curr_client.store(tmp_client);
    }

    void handleMessageSend()
    {
        if (curr_client.load() && curr_client.load()->wsi)
        {
            lws_callback_on_writable(curr_client.load()->wsi);
        }
        else
        {
            qDebug("Erorr in handleMessageSend");
        }
    }

private:
    QLineEdit *inputLineEdit;
};

#endif // CUSTOM_ELEMENTS_H