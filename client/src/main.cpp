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

    // Connecting to chat server
    QUrl url("ws://localhost:8000/ws");
    ClientConnection client(url);

    QThread serverThread;
    QObject::connect(&serverThread, &QThread::started, &client, &ClientConnection::start);
    serverThread.start();

    // Start a LoginWindow instance
    LoginWindow loginWindow;
    loginWindow.show();

    return app.exec();
}