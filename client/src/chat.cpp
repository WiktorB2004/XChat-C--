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
    leftColumnLayout = new QVBoxLayout();

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
    messageLineEdit->setText("");
    updateMessageList();
}

void ChatWindow::handleMessageRecieve(Message msg)
{
    message_list.push_back(msg);
    updateMessageList();
    refreshClientList();
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
    std::vector<Message> latest_messages;
    int messages_to_show = 10;
    int containerHeight = messageListVBoxLayout->geometry().height();
    if (message_list.size() > messages_to_show)
    {
        latest_messages = std::vector<Message>(message_list.end() - messages_to_show, message_list.end());
    }
    else
    {
        latest_messages = message_list;
    }

    for (Message msg : latest_messages)
    {
        // FIXME: Make it use correct username
        curr_message = new QLabel(msg.content + " - " + msg.sender_username);
        curr_message->setAlignment((msg.sender_username.toStdString() == client_username.toStdString() ? Qt::AlignRight : Qt::AlignLeft) | Qt::AlignBottom);
        // FIXME: Make resizing dynamic
        curr_message->setFixedHeight(40);
        messageListVBoxLayout->addWidget(curr_message);
    }
    messageListVBoxLayout->setAlignment(Qt::AlignBottom);
}

void ChatWindow::syncServerData(std::vector<QString> client_list, std::vector<QString> message_data)
{
    m_client_list = client_list;
    m_message_data = message_data;
    refreshClientList();
    updateMessageList();
}

void ChatWindow::refreshClientList()
{
    // Clear the existing messages
    QLayoutItem *child;
    while ((child = leftColumnLayout->takeAt(0)) != nullptr)
    {
        delete child->widget();
        delete child;
    }

    // Add the latest messages
    QLabel *client;
    std::vector<QString> top_clients;
    int clients_to_show = 10;
    int containerHeight = leftColumnLayout->geometry().height();
    if (m_client_list.size() > clients_to_show)
    {
        top_clients = std::vector<QString>(m_client_list.end() - clients_to_show, m_client_list.end());
    }
    else
    {
        top_clients = m_client_list;
    }

    for (QString username : top_clients)
    {
        if (username != client_username)
        {
            client = new QLabel(username);
            client->setAlignment(Qt::AlignCenter);
            // FIXME: Make resizing dynamic
            client->setFixedHeight(40);
            leftColumnLayout->addWidget(client);
        }
    }
    leftColumnLayout->setAlignment(Qt::AlignTop);
}

ChatWindow::~ChatWindow()
{
}