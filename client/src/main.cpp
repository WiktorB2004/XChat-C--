#include <QCoreApplication>
#include <QDebug>
#include <QFontDatabase>
#include <QFont>
#include <QGuiApplication>
#include <QStringList>
#include <QThread>
#include "../include/app_windows.h"
#include "../include/chat_connection.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Load font from assets and set it as application default
    QString fontFilePath = QApplication::applicationDirPath() + "/../../assets/fonts/Roboto/Roboto-Regular.ttf";
    int fontId = QFontDatabase::addApplicationFont(fontFilePath);
    if (fontId == -1)
    {
        qDebug() << "Failed to load font from file" << fontFilePath;
    }
    else
    {
        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
        if (!fontFamilies.isEmpty())
        {
            QApplication::setFont(QFont(fontFamilies.first(), 10)); // Set your desired font family and size
        }
        else
        {
            qDebug() << "Failed to retrieve font family";
        }
    }

    // Start a LoginWindow instance and wait for the user to input connection data
    LoginWindow loginWindow;
    loginWindow.show();

    // Show login window and validate user
    QEventLoop eventLoop;
    QObject::connect(&loginWindow, &LoginWindow::loginSuccess, &eventLoop, &QEventLoop::quit);
    QObject::connect(&loginWindow, &LoginWindow::loginSuccess, &loginWindow, &LoginWindow::close);
    eventLoop.exec();

    // Connect to chat server and start chat window
    QString client_username = loginWindow.m_inputData.object().value("username").toString();
    QUrl url("ws://" + loginWindow.m_inputData.object().value("serverIp").toString() + "/ws");
    ChatWindow chatWindow(client_username);
    ClientConnection client(url, client_username);
    QThread serverThread;
    // Start the connection related thread and window
    QObject::connect(&serverThread, &QThread::started, &client, &ClientConnection::start);
    QObject::connect(&client, &ClientConnection::connectionSuccess, &chatWindow, &ChatWindow::show);
    // Handle connection errors
    QObject::connect(&client, &ClientConnection::connectionFailure, &app, &QApplication::quit);
    QObject::connect(&client, &ClientConnection::connectionFailure, &serverThread, &QThread::quit);
    // Close the thread after closing window
    QObject::connect(&chatWindow, &ChatWindow::close, &serverThread, &QThread::quit);

    // Handle message sending
    QObject::connect(&chatWindow, &ChatWindow::sendMessage, &client, &ClientConnection::sendMessage);
    QObject::connect(&chatWindow, &ChatWindow::close, &serverThread, &QThread::quit);
    // Handle message recieving
    QObject::connect(&client, &ClientConnection::recievedMessage, &chatWindow, &ChatWindow::handleMessageRecieve);
    serverThread.start();

    serverThread.quit();
    return app.exec();
}