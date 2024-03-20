#ifndef APP_WINDOWS_H
#define APP_WINDOWS_H

#include <QApplication>
#include <QCoreApplication>
#include <QMainWindow>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QScreen>
#include <QPixmap>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include "message.h"

/**
 * @brief The LoginWindow class
 *
 * This class represents the login window of the application.
 */
class LoginWindow : public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new Login Window object
     *
     * @param parent The parent widget.
     */
    explicit LoginWindow(QWidget *parent = nullptr);

    /**
     * @brief Destroy the Login Window object
     *
     */
    ~LoginWindow();

    /**
     * @brief The data of each user input in JSON format
     *
     * It stores values of username, password and serever ip.
     */
    QJsonDocument m_inputData;

signals:
    /**
     * @brief Signal emitted when login is successful.
     *
     */
    void loginSuccess();

    /**
     * @brief Signal emitted when login is unsuccessful.
     *
     */
    void loginFailure();

private slots:
    /**
     * @brief Slot to update the input data after any change.
     *
     */
    void updateInputData();

    /**
     * @brief Slot to handle login button submission.
     *
     */
    void loginSubmit();

private:
    /**
     * @brief Line edit for server IP.
     *
     */
    QLineEdit *serverIpLineEdit;

    /**
     * @brief Line edit for username.
     *
     */
    QLineEdit *usernameLineEdit;

    /**
     * @brief Line edit for password.
     *
     */
    QLineEdit *passwordLineEdit;

    /**
     * @brief Submit button for login.
     *
     */
    QPushButton *submitButton;
};

/**
 * @brief The ChatWindow class
 *
 * This class represents the chat window of the application.
 */
class ChatWindow : public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new Chat Window object
     *
     * @param client_username The username of the client.
     * @param parent The parent widget.
     */
    explicit ChatWindow(QString client_username, QWidget *parent = nullptr);

    /**
     * @brief Destroy the Chat Window object
     *
     */
    ~ChatWindow();

    /**
     * @brief The value of line edits inside chat window.
     *
     */
    QJsonObject m_inputData;

    /**
     * @brief The list of messages.
     *
     */
    std::vector<Message> message_list;

    /**
     * @brief The list of the clients.
     *
     */
    std::vector<QString> m_client_list;

    /**
     * @brief The list of the messages.
     */
    std::vector<QString> m_message_data;

    /**
     * @brief Refresh the message list.
     *
     */
    void updateMessageList();

    /**
     * @brief Refresh the client list.
     *
     */
    void refreshClientList();
signals:
    /**
     * @brief Signal to send a message.
     *
     * @param msg The message to send.
     */
    void sendMessage(QJsonObject msg);

    /**
     * @brief Signal to sync the chat switch.
     *
     * @param name The name of the chat
     */
    void chatSwitchSync(QString name);

public slots:
    /**
     * @brief Slot to handle received messages.
     *
     */
    void handleMessageRecieve();

    /**
     * @brief Slot to sync client list of the server.
     *
     * @param client_list The list of the clients.
     * @param message_data The list of the messages.
     */
    void syncServerData(std::vector<QString> client_list, std::vector<Message> message_data);
private slots:
    /**
     * @brief Slot to update the input data.
     *
     */
    void updateInputData();

    /**
     * @brief Slot to handle sending messages.
     *
     */
    void handleMessageSend();

    /**
     * @brief Slot to swtich the chat
     *
     * @param usernamename The name of the chat
     */
    void switchChat(QString username);

private:
    /**
     * @brief Line edit for message input.
     *
     */
    QLineEdit *messageLineEdit;

    /**
     * @brief Layout for the message list.
     *
     */
    QVBoxLayout *messageListVBoxLayout;

    /**
     * @brief Layout for the client list.
     *
     */
    QVBoxLayout *leftColumnLayout;

    /**
     * @brief The username of the client.
     *
     */

    QString client_username;
};

#endif // APP_WINDOWS_H