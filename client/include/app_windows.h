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
#include <QDesktopWidget>
#include <QScreen>
#include <QPixmap>
#include <QDebug>
#include "../include/server_connection.h"

class LoginWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit LoginWindow(ServerConnection *connection, QWidget *parent = nullptr);
    virtual ~LoginWindow();
};

class ChatWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ChatWindow(ServerConnection *connection, QWidget *parent = nullptr);
    virtual ~ChatWindow();

    ServerConnection *connection;

private:
};

#endif // APP_WINDOWS_H