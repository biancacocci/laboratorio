//
// Created by Bianca Cocci on 02/03/25.
//

#ifndef LABORATORIO_CHAT_H
#define LABORATORIO_CHAT_H
#include "user.h"
#include "message.h"
#include <list>
#include <stdexcept>

// Eccezione per la cancellazione di un messaggio letto
class MessageAlreadyReadException : public std::runtime_error {
public:
    explicit MessageAlreadyReadException(const std::string& message)
            : std::runtime_error(message) {}
};

class chat {
public:
    chat(user& user1, user& user2);

    const user &getUser1() const;

    const user &getUser2() const;

    const std::list<message*> &getMessages() const;

    void addMessage(message& message);

    void removeMessage(const message& msg);


    bool hasUser(const user& u) const;

    message* findMessageByText(const std::string& text) const;
    void forwardMessage(const message& msg, user& targetUser);

    ~chat();

private:
    user user1;
    user user2;
    std:: list <message*> messages;

};


#endif //LABORATORIO_CHAT_H
