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

ChatWindow::ChatWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Create a vertical layout for left column
    QVBoxLayout *leftColumnLayout = new QVBoxLayout();
    QLabel *leftColumnLabel = new QLabel("Left Column");
    leftColumnLabel->setAlignment(Qt::AlignCenter);
    leftColumnLabel->setStyleSheet("QLabel { background-color: rgba(0, 0, 0, 128); color: white; }"); // Set transparent background
    leftColumnLayout->addWidget(leftColumnLabel);

    // Create a vertical layout for right column
    QVBoxLayout *rightColumnLayout = new QVBoxLayout();
    messageLineEdit = new QLineEdit();
    rightColumnLayout->addWidget(messageLineEdit);

    // Create a horizontal layout for the right column
    QHBoxLayout *rightColumnHBoxLayout = new QHBoxLayout();
    rightColumnHBoxLayout->addLayout(rightColumnLayout);

    // Create a button
    QPushButton *sendButton = new QPushButton("Send");
    rightColumnHBoxLayout->addWidget(sendButton);

    // Create a widget for each layout
    QWidget *leftColumnWidget = new QWidget();
    QWidget *rightColumnWidget = new QWidget();

    // Set layouts for the widgets
    leftColumnWidget->setLayout(leftColumnLayout);
    rightColumnWidget->setLayout(rightColumnHBoxLayout);

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
}

ChatWindow::~ChatWindow()
{
}