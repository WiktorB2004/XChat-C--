#ifndef MESSAGE_H
#define MESSAGE_H
#include <vector>
#include <QString>

class Message
{
public:
    Message(QString sender_username, QString content)
        : sender_username(sender_username), content(content)
    {
    }

    QString sender_username;
    QString content;
};

#endif // MESSAGE_H