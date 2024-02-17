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
#include "../include/chat_connection.h"

class LoginWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private:
    QLabel *backgroundLabel;
    QLineEdit *serverIpLineEdit;
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *submitButton;
    QVBoxLayout *formLayout;
    QHBoxLayout *formMenuLayout;
    QWidget *formMenuWidget;
    QWidget *formWidget;
    QVBoxLayout *centerLayout;
    QWidget *centralWidget;
};

class ChatWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ChatWindow(QWidget *parent = nullptr);
    virtual ~ChatWindow();

private:
};

#endif // APP_WINDOWS_H