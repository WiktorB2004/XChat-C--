#include <QtCore/QDebug>
#include <QtWebSockets>
#include <QtCore>
#include <algorithm>
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
    if (client_username.isEmpty())
    {
        qDebug() << "User passed empty username";
        emit connectionFailure();
        return;
    }
    QJsonObject msg;
    msg["sender"] = client_username;
    msg["content"] = "New connection";
    msg["type"] = "message";
    QJsonDocument doc(msg);
    QByteArray data = doc.toJson();
    m_client.sendTextMessage(data);
    emit connectionSuccess();
    qDebug() << "Connected to server";
}

void ClientConnection::sendMessage(QJsonObject msg)
{
    msg["sender"] = client_username;
    msg["type"] = "message";
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

    if (msg_json.value("type").toString() == "data")
    {
        // Get the "content" object from msg_json
        QJsonObject content = msg_json.value("content").toObject();
        // Get the "users" array from the "content" object
        QJsonArray users_array = content.value("users").toArray();
        // Convert the QJsonArray to a std::vector<QString>
        std::vector<QString> client_list;
        for (const auto &user : users_array)
        {
            client_list.push_back(user.toObject().value("username").toString());
        }
        // Get the "messages" array from the "content" object
        QJsonArray messages_array = content.value("messages").toArray();
        // Convert the QJsonArray to a std::vector<QString>
        std::vector<Message> message_list;
        for (const auto &message : messages_array)
        {
            QJsonObject msgObj = message.toObject();
            QString sender_username = msgObj.value("sender").toString();
            QString message_content = msgObj.value("content").toString();
            Message msg(sender_username, message_content, "message");
            message_list.push_back(msg);
        }
        m_client_list = client_list;
        qDebug() << "Received data from server:" << message;
        emit syncData(client_list, message_list);
    }
    else if (msg_json.value("type").toString() != "switch")
    {
        // Message msg(msg_json.value("sender").toString(), msg_json.value("content").toString(), "message");
        qDebug() << "Received message from server:" << message;
        emit recievedMessage();
    }
}

void ClientConnection::handleChatSwitch(QString name)
{
    QJsonObject msg;
    msg["sender"] = client_username;
    msg["content"] = name;
    msg["type"] = "switch";
    QJsonDocument doc(msg);
    QByteArray data = doc.toJson();
    m_client.sendTextMessage(data);
}
