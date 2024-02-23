#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

#include <QtCore/QObject>
#include <QtWebSockets>
#include "message.h"

class ClientConnection : public QObject
{
    Q_OBJECT
public:
    explicit ClientConnection(QUrl url, QString client_username, QObject *parent = nullptr);
    void start();
    ~ClientConnection() override;

protected:
    QWebSocket m_client;
    QUrl m_url;

    friend class ServerThread;

private:
    QString client_username;
signals:
    void connectionSuccess();
    void connectionFailure();
    void recievedMessage(Message msg);
public slots:
    void sendMessage(QJsonObject msg);
private slots:
    void onConnected();
    void onError(QAbstractSocket::SocketError error);
    void onTextMessageReceived(const QString &message);
};

#endif // SERVER_CONNECTION_H