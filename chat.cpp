#include "user.h"
#include "chat.h"
#include <iostream>

chat::chat(const user& user1, const  user& user2): user1(user1), user2(user2) {}


const user &chat::getUser1() const {
    return user1;
}


const user &chat::getUser2() const {
    return user2;
}

const message* chat::getLastMessage() const {
    return messages.empty() ? nullptr : messages.back();
}


void chat::forEachMessage(std::function<void(const message&)> callback) const {
    for (const auto& msg : messages) {
        callback(*msg);
    }
}



void chat::addMessage( message &msg) {
    messages.push_back(&msg);
}


void chat::removeMessage(const message& msg) {
    if (msg.getIsRead()) {
        throw MessageAlreadyReadException("Impossibile rimuovere un messaggio già letto.");
    }


    messages.remove_if([&](const message* m) {
        return m->getSender() == msg.getSender() && m->getReceiver() == msg.getReceiver() && m->getText() == msg.getText();
    });
}


std::list<message*> chat::findMessageByText(const std::string& text) const {
    std::list<message*> foundMessages;
    for (auto& msg : messages) {
        if (msg->getText().find(text) != std::string::npos) {
            foundMessages.push_back(msg);
        }
    }
    return foundMessages;
}




void chat::forwardMessage(const message& msg, user& targetUser) {
    if (findMessageByText(msg.getText()).empty()) {
        throw std::invalid_argument("Il messaggio specificato non esiste nella chat.");
    }

    message* forwardedMessage = new message(
            msg.getSender(),
            targetUser,
            "Inoltrato: " + msg.getText()
    );

    addMessage(*forwardedMessage);

    std::cout << "Messaggio inoltrato con successo da " << msg.getSender().getName() << " a " << targetUser.getName() << std::endl;
}



chat::~chat() {
    for (auto& msg : messages) {
        delete msg;
    }
    messages.clear();
}











