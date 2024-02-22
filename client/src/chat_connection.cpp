#include <QtCore/QDebug>
#include <QtWebSockets>
#include <QtCore>
#include "../include/chat_connection.h"

using namespace std;

QT_USE_NAMESPACE

ClientConnection::ClientConnection(QUrl url, QObject *parent) : m_url(url), QObject(parent)
{
}

void ClientConnection::start()
{
    // Connect to the WebSocket server or handle errors
    QObject::connect(&m_client, &QWebSocket::connected, this, &ClientConnection::onConnected);
    QObject::connect(&m_client, &QWebSocket::textMessageReceived, this, &ClientConnection::onTextMessageReceived);
    QObject::connect(&m_client, &QWebSocket::errorOccurred, this, &ClientConnection::onError);

    m_client.open(m_url);
}

void ClientConnection::onConnected()
{
    qDebug() << "Connected to server";
    // Create a JSON message
    QJsonObject message;
    message["sender"] = "Client";
    message["content"] = "Connected to server";
    QJsonDocument doc(message);
    QByteArray data = doc.toJson();
    // Send the message to the server
    m_client.sendTextMessage(QString::fromUtf8(data));
    emit connectionSuccess();
}

void ClientConnection::sendMessage(QJsonObject msg)
{
    msg["sender"] = "Client";
    QJsonDocument doc(msg);
    QByteArray data = doc.toJson();
    m_client.sendTextMessage(data);
}

void ClientConnection::onError(QAbstractSocket::SocketError error)
{
    qDebug() << "Error occurred during connection:" << error;
    emit connectionFailure();
}

ClientConnection::~ClientConnection()
{
}

void ClientConnection::onTextMessageReceived(const QString &message)
{
    qDebug() << "Received message from server:" << message;
}
