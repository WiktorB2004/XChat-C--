#ifndef APP_WINDOWS_H
#define APP_WINDOWS_H

#include <QApplication>
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


class LoginWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit LoginWindow(QWidget *parent = nullptr);
    virtual ~LoginWindow();
};

class ChatWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ChatWindow(QWidget *parent = nullptr);
    virtual ~ChatWindow();
};

#endif // APP_WINDOWS_H