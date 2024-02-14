#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

#include <string>
#include <iostream>
#include <libwebsockets.h>
#include <thread>
#include <chrono>
#include <string>
#include "client.h"

class ServerConnection : public QObject
{
    Q_OBJECT
public:
    ServerConnection(Client &client, std::string address, int port)
        : client(client), server_address(address), connection_port(port) {}
    Client &client;
    std::string server_address;
    int connection_port;

public slots:
    int run_server();
    int send_message();

signals:
    // FIXME: Create singals to handle server status management
};

#endif // SERVER_CONNECTION_H