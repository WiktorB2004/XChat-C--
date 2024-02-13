#include <thread>
#include "../include/app_windows.h"
#include "../include/server_connection.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Client client;
    client.username = "Zdzipek";
    client.message = "TEST";
    client.address = "localhost";
    client.port = 8000;
    client.setStatus(CLOSED);

    // Launch the app window in a separate thread
    std::thread t1(start_connection, std::ref(client), "localhost", 8000);
    t1.detach();

    // Launch the menu window
    LoginWindow login_window;
    login_window.show();

    // Launch chat window after establishing server connections
    ChatWindow chat_window;
    QObject::connect(&client, &Client::connectionEstablished, [&]()
                     {
                         if (client.getStatus() == OPEN)
                         {
                             chat_window.show();
                         } //
                     });

    return app.exec();
}