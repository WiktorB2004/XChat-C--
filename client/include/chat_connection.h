#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

#include <QtCore/QObject>
#include <QtWebSockets>
#include "message.h"

/**
 * @brief The ClientConnection class
 *
 * This class represents a connection to a server using WebSocket protocol.
 */
class ClientConnection : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief ClientConnection constructor.
     *
     * @param url The URL of the server.
     * @param client_username The username of the client.
     * @param parent The parent QObject.
     */
    explicit ClientConnection(QUrl url, QString client_username, QObject *parent = nullptr);

    /**
     * @brief Starts the connection to the server.
     */
    void start();

    /**
     * @brief ClientConnection destructor.
     */
    ~ClientConnection() override;

protected:
    QWebSocket m_client; /**< The WebSocket client. */
    QUrl m_url;          /**< The URL of the server. */

    friend class ServerThread;
private:
    QString client_username; /**< The username of the client. */

signals:
    /**
     * @brief Signal emitted when the connection to the server is successful.
     */
    void connectionSuccess();

    /**
     * @brief Signal emitted when the connection to the server fails.
     */
    void connectionFailure();

    /**
     * @brief Signal emitted when a message is received from the server.
     *
     * @param msg The received message.
     */
    void recievedMessage(Message msg);

public slots:
    /**
     * @brief Slot to send a message to the server.
     *
     * @param msg The message to send.
     */
    void sendMessage(QJsonObject msg);

private slots:
    /**
     * @brief Slot called when the connection to the server is established.
     */
    void onConnected();

    /**
     * @brief Slot called when an error occurs in the connection.
     *
     * @param error The error code.
     */
    void onError(QAbstractSocket::SocketError error);

    /**
     * @brief Slot called when a text message is received from the server.
     *
     * @param message The received message.
     */
    void onTextMessageReceived(const QString &message);
};

#endif // SERVER_CONNECTION_H
