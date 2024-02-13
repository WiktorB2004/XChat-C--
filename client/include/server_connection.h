#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

#include <string>
#include <iostream>
#include <libwebsockets.h>
#include <thread>
#include <chrono>
#include <string>

// Define the state of the WebSocket connection
enum ConnectionState
{
    CONNECTING,
    OPEN,
    CLOSING,
    CLOSED
};

// ------------ TO BE MOVED + ADJUST CMAKE MOC ------------

#include <string>
#include <QObject>

class Client : public QObject
{
    Q_OBJECT
public:
    std::string username;
    std::string address;
    std::string message;
    int port;
    int message_count;

    void setStatus(ConnectionState new_status)
    {
        if (status != new_status)
        {
            status = new_status;
            emit statusChanged();
        }
    }

    ConnectionState getStatus() const
    {
        return status;
    }

signals:
    // FIXME: Implement signals to properly handle multi-threaded operations and communictaion with server
    void statusChanged();
    void connectionEstablished();

private:
    std::string password;
    std::string client_ip;
    enum ConnectionState status = CLOSED;
};

// ------------ ^TO BE MOVED + ADJUST CMAKE MOC ------------

int start_connection(Client &client, std::string server_address, int connection_port);

#endif // SERVER_CONNECTION_H