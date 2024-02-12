#include <thread>
#include <QApplication>
#include "../include/app_windows.h"
#include "../include/server_connection.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Launch the app window in a separate thread
    std::thread t1(server_start, "localhost", 8000);

    // Launch the menu window
    menu_window(app, argc, argv);
    // Launch chat window
    chat_window(app, argc, argv);

    t1.join();

    return 0;
}