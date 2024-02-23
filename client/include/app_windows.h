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

class LoginWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
    QJsonDocument m_inputData;

signals:
    void loginSuccess();
    void loginFailure();

private slots:
    void updateInputData();
    void loginSubmit();

private:
    QLineEdit *serverIpLineEdit;
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *submitButton;
};

class ChatWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ChatWindow(QString client_username, QWidget *parent = nullptr);
    ~ChatWindow();
    QJsonObject m_inputData;
    void updateMessageList();
    std::vector<Message> message_list;


signals:
    void sendMessage(QJsonObject msg);

public slots:
    void handleMessageRecieve(Message msg);

private slots:
    void updateInputData();
    void handleMessageSend();

private:
    QLineEdit *messageLineEdit;
    QVBoxLayout *messageListVBoxLayout;
    QString client_username;
};

#endif // APP_WINDOWS_H