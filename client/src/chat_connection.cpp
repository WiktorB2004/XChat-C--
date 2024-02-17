#include <QtCore/QDebug>
#include <QtWebSockets>
#include <QtCore>
#include "../include/chat_connection.h"

using namespace std;

QT_USE_NAMESPACE

ClientConnection::ClientConnection(QUrl url, QObject *parent) : url(url), QObject(parent)
{
}

void ClientConnection::start()
{
    client.open(url);
    // Connect to the WebSocket server
    QObject::connect(&client, &QWebSocket::connected, this, &ClientConnection::onConnected);
    QObject::connect(&client, &QWebSocket::textMessageReceived, this, &ClientConnection::onTextMessageReceived);
}

void ClientConnection::onConnected()
{
    qDebug() << "Connected to server";
    // Create a JSON message
    QJsonObject message;
    message["sender"] = "Client";
    message["content"] = "Hello, server!";
    QJsonDocument doc(message);
    QByteArray data = doc.toJson();
    // Send the message to the server
    client.sendTextMessage(QString::fromUtf8(data));
}

ClientConnection::~ClientConnection()
{
}

void ClientConnection::onTextMessageReceived(const QString &message)
{
    qDebug() << "Received message from server:" << message;
}
