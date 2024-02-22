#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

#include <QtCore/QObject>
#include <QtWebSockets>
#include "threads.h"

class ClientConnection : public QObject
{
    Q_OBJECT
public:
    explicit ClientConnection(QUrl url, QObject *parent = nullptr);
    void start();
    ~ClientConnection() override;

protected:
    QWebSocket m_client;
    QUrl m_url;

    friend class ServerThread;
signals:
    void connectionSuccess();
    void connectionFailure();
public slots:
    void sendMessage(QJsonObject msg);
private slots:
    void onConnected();
    void onError(QAbstractSocket::SocketError error);
    void onTextMessageReceived(const QString &message);
};

#endif // SERVER_CONNECTION_H