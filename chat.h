//
// Created by Bianca Cocci on 02/03/25.
//

#ifndef LABORATORIO_CHAT_H
#define LABORATORIO_CHAT_H
#include "user.h"
#include "message.h"
#include <list>
#include <stdexcept>
#include <string>

class MessageAlreadyReadException : public std::runtime_error {
public:
    explicit MessageAlreadyReadException(const std::string& message)
            : std::runtime_error(message) {}
};

class chat {
public:
    chat(const user& user1, const user& user2, const std::string& chatName);

    const user &getUser1() const;

    const user &getUser2() const;

    const std::string& getChatName() const;

    std::pair<int, int> forEachMessage(std::function<void(const message&)> callback) const;

    void addMessage(const message& msg, bool isForwarded = false);

    void removeMessage(const message& msg);

    int getTotalMessagesCount() const;

    int getUnreadMessagesCount() const;

    std::pair<int, int> markMessagesAsRead(int n);

    const std::shared_ptr<message> getLastMessage() const;

    std::list<std::shared_ptr<message>> findMessageByText(const std::string& text) const;

    void forwardMessage(const message& msg,user& targetUser);

    ~chat();

private:
    const user user1;
    const user user2;
    const std::string chatName;
    std::list<std::shared_ptr<message>> messages;
};

#endif //LABORATORIO_CHAT_H



