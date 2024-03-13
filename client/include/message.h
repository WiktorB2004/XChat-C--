#ifndef MESSAGE_H
#define MESSAGE_H
#include <vector>
#include <QString>

class Message
{
public:
    Message(QString sender_username, QString content, QString type)
        : sender_username(sender_username), content(content), type(type)
    {
    }

    QString sender_username;
    QString type;
    QString content;
};

#endif // MESSAGE_H