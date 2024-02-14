#include <QThread>
#include "../include/app_windows.h"
#include "../include/server_connection.h"
#include "../include/client.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create and configure the client
    Client client("Zdzipek", "password", "localhost", 8000);
    std::string connection_address = client.select_address + std::string(":") + std::to_string(client.select_port);
    client.input = "Hello my name is Zdzipek";
    std::string message = "{\"Sender\":\"" + client.username + "\",\"Recipient\":\"" + connection_address + "\",\"content\":\"Message sent by client:" + client.input + "\"}";
    client.message = message;

    // Create a QThread instance for the server connection
    QThread serverThread;

    // Create and move the server connection object to the new thread
    ServerConnection serverConnection(client, "localhost", 8000);
    serverConnection.moveToThread(&serverThread);

    // Connect the thread's started() signal to the run_server function and start the thread
    QObject::connect(&serverThread, &QThread::started, &serverConnection, &ServerConnection::run_server);
    serverThread.start();

    // Launch the login window
    LoginWindow login_window;
    login_window.show();

    // Launch the chat window after successfully connecting to the server
    ChatWindow chat_window;
    QObject::connect(&client, &Client::connectionSuccess, &chat_window, &ChatWindow::show);

    QObject::connect(&client, &Client::sendMessage, &serverConnection, &ServerConnection::send_message);

    return app.exec();
}