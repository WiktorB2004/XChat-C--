#include <QThread>
#include <QDebug>
#include <QFontDatabase>
#include <QFont>
#include "../include/app_windows.h"
#include "../include/server_connection.h"
#include "../include/client.h"

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    QString fontFilePath = QCoreApplication::applicationDirPath() + "/../../assets/fonts/Roboto/Roboto-Regular.ttf";
    int fontId = QFontDatabase::addApplicationFont(fontFilePath);
    if (fontId == -1)
    {
        qDebug() << "Failed to load font from file" << fontFilePath;
    }

    // Set default font for the application
    if (fontId != -1)
    {
        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
        if (!fontFamilies.isEmpty())
        {
            QGuiApplication::setFont(QFont("Arial", 10)); // Set your desired font family and size
        }
        else
        {
            qDebug() << "Failed to retrieve font family";
        }
    }

    // Create and configure the client
    Client client("Zdzipek", "password", "localhost", 8000);

    // ----- TO BE MOVED/DELETED -----
    std::string connection_address = client.select_address + std::string(":") + std::to_string(client.select_port);
    client.input = "Hello my name is Zdzipek";
    std::string message = "{\"Sender\":\"" + client.username + "\",\"Recipient\":\"" + connection_address + "\",\"content\":\"Message sent by client:" + client.input + "\"}";
    client.message = message;
    // ----- TO BE MOVED/DELETED -----

    // Create a QThread instance for the server connection
    QThread serverThread;

    // Create and move the server connection object to the new thread
    ServerConnection serverConnection(&client, "localhost", 8000);
    serverConnection.moveToThread(&serverThread);

    // Connect the thread's started() signal to the run_server function and start the thread
    QObject::connect(&serverThread, &QThread::started, &serverConnection, &ServerConnection::run_server);
    serverThread.start();

    // Launch the login window
    LoginWindow login_window(&serverConnection);
    login_window.show();

    // Launch the chat window after successfully connecting to the server
    ChatWindow chat_window(&serverConnection);
    QObject::connect(&client, &Client::connectionSuccess, &chat_window, &ChatWindow::show);

    return app.exec();
}