//
// Created by Bianca Cocci on 02/03/25.
//

#ifndef LABORATORIO_CHAT_H
#define LABORATORIO_CHAT_H
#include "user.h"
#include "message.h"
#include <list>
#include <stdexcept>


class MessageAlreadyReadException : public std::runtime_error {
public:
    explicit MessageAlreadyReadException(const std::string& message)
            : std::runtime_error(message) {}
};

class chat {
public:
    chat(const user& user1, const user& user2);

    const user &getUser1() const;

    const user &getUser2() const;

    void forEachMessage(std::function<void(const message&)> callback) const;

    void addMessage(message& message);

    void removeMessage(const message& msg);

    const message* getLastMessage() const;



    std::list<message*> findMessageByText(const std::string& text) const;

    void forwardMessage(const message& msg, user& targetUser);

    ~chat();

private:
    const user user1;
    const user user2;
    std:: list <message*> messages;

};


#endif //LABORATORIO_CHAT_H
