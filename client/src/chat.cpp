#include <QGuiApplication>
#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QScreen>
#include <QSplitter>
#include "../include/app_windows.h"
#include "../include/custom_qelements.h"
#include "../include/chat_connection.h"
#include "../include/message.h"

ChatWindow::ChatWindow(QString client_username, QWidget *parent)
    : client_username(client_username), QMainWindow(parent)
{
    // Create a vertical layout for left column
    QVBoxLayout *leftColumnLayout = new QVBoxLayout();
    QLabel *leftColumnLabel = new QLabel("Left Column");
    leftColumnLabel->setAlignment(Qt::AlignCenter);
    leftColumnLabel->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 128); color: white; }"); // Set transparent background
    leftColumnLayout->addWidget(leftColumnLabel);

    // Create a vertical layout for right column
    QVBoxLayout *rightColumnLayout = new QVBoxLayout();

    // Create vertical layout for messages
    messageListVBoxLayout = new QVBoxLayout();

    // Create a horizontal layout for the message send
    QHBoxLayout *messageSendHBoxLayout = new QHBoxLayout();
    // Create a message panel
    QPushButton *sendButton = new QPushButton("Send");
    QLabel *messageLabel = new QLabel("Message");

    messageLineEdit = new QLineEdit();
    messageSendHBoxLayout->addWidget(messageLabel);
    messageSendHBoxLayout->addWidget(messageLineEdit);
    messageSendHBoxLayout->addWidget(sendButton);

    // Set layouts for right column
    rightColumnLayout->addLayout(messageListVBoxLayout);
    rightColumnLayout->addLayout(messageSendHBoxLayout);

    // Create a widget for each layout
    QWidget *leftColumnWidget = new QWidget();
    QWidget *rightColumnWidget = new QWidget();

    // Set layouts for the widgets
    leftColumnWidget->setLayout(leftColumnLayout);
    rightColumnWidget->setLayout(rightColumnLayout);

    // Create a splitter
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(leftColumnWidget);
    splitter->addWidget(rightColumnWidget);

    setCentralWidget(splitter);

    // Center the window on the screen
    setGeometry(
        QGuiApplication::primaryScreen()->geometry().center().x() - width() / 2,
        QGuiApplication::primaryScreen()->geometry().center().y() - height() / 2,
        width(),
        height());

    QObject::connect(messageLineEdit, &QLineEdit::textChanged, this, &ChatWindow::updateInputData);
    QObject::connect(sendButton, &QPushButton::clicked, this, &ChatWindow::handleMessageSend);
}

void ChatWindow::updateInputData()
{
    QJsonObject message;
    message["content"] = messageLineEdit->text();
    m_inputData = message;
}

void ChatWindow::handleMessageSend()
{
    // Perform some validation
    emit sendMessage(m_inputData);
    updateMessageList();
}

void ChatWindow::handleMessageRecieve(Message msg)
{
    message_list.push_back(msg);
    updateMessageList();
}

void ChatWindow::updateMessageList()
{
    // Clear the existing messages
    QLayoutItem *child;
    while ((child = messageListVBoxLayout->takeAt(0)) != nullptr)
    {
        delete child->widget();
        delete child;
    }

    // Add the latest messages
    QLabel *curr_message;
    for (Message msg : message_list)
    {
        // FIXME: Make it use correct username
        curr_message = new QLabel(msg.content + " - " + msg.sender_username);
        curr_message->setAlignment(msg.sender_username.toStdString() == client_username.toStdString() ? Qt::AlignRight : Qt::AlignLeft);
        messageListVBoxLayout->addWidget(curr_message);
    }
}

ChatWindow::~ChatWindow()
{
}