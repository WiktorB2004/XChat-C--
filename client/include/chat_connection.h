#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

#include <QtCore/QObject>
#include <QtWebSockets>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)
QT_FORWARD_DECLARE_CLASS(QString)

class ClientConnection : public QObject
{
    Q_OBJECT
public:
    
    explicit ClientConnection(QUrl url, QObject *parent = nullptr);
    void start();
    ~ClientConnection() override;

private slots:
    void onTextMessageReceived(const QString &message);
    void onConnected();

private:
    QWebSocket client;
    QUrl url;
};

#endif // SERVER_CONNECTION_H