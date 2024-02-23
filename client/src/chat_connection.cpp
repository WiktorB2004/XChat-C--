#include <QtCore/QDebug>
#include <QtWebSockets>
#include <QtCore>
#include "../include/chat_connection.h"

using namespace std;

QT_USE_NAMESPACE

ClientConnection::ClientConnection(QUrl url, QString client_username, QObject *parent) : m_url(url), client_username(client_username), QObject(parent)
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
    emit connectionSuccess();
}

void ClientConnection::sendMessage(QJsonObject msg)
{
    msg["sender"] = client_username;
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
    QJsonObject msg_json;
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());

    if (!doc.isNull())
    {
        if (doc.isObject())
        {
            msg_json = doc.object();
        }
        else
        {
            qDebug() << "Document is not an object";
        }
    }
    else
    {
        qDebug() << "Invalid JSON: " << message;
    }

    Message msg(msg_json.value("sender").toString(), msg_json.value("content").toString());
    emit recievedMessage(msg);
}
