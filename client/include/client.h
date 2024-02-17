#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <QObject>
#include <libwebsockets.h>
#include <atomic>
#include <mutex>

class Client : public QObject
{
    Q_OBJECT
public:
    Client(std::string username, std::string password, std::string address, int port)
        : username(username), password(password), select_address(address), select_port(port), wsi(nullptr) {}
    std::string username;
    std::string password;
    std::string message;
    std::string input;
    std::string select_address;
    int select_port;

    struct lws *wsi;

signals:
    // FIXME: Create signals to handle user data - inputs, GUI operations etc.
    void connectionSuccess();
    void sendMessage();

private:
    //  FIXME: Move here variables that should be private
};

#endif // CLIENT_H