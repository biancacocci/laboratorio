//
// Created by Bianca Cocci on 02/03/25.
//

#ifndef LABORATORIO_MESSAGE_H
#define LABORATORIO_MESSAGE_H
#include "user.h"
#include <stdexcept>

class EmptyMessageException : public std::runtime_error {
public:
    explicit EmptyMessageException(const std::string& message)
        : std::runtime_error(message) {}
};

class message {
public:
    message(const user& sender, const user& receiver, const std::string& text);

    void setText(const std::string &newtext);

    const user &getReceiver() const;

    const user &getSender() const;

    const std::string &getText() const;

    bool IsRead() const;

    void markAsRead();

    bool operator==(const message& other) const;

private:
    user sender;
    user receiver;
    std:: string text;
    bool isRead=false;
};

#endif //LABORATORIO_MESSAGE_H
