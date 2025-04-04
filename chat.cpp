#include "user.h"
#include "chat.h"
#include <iostream>

chat::chat(const user& user1, const user& user2, const std::string& chatName): user1(user1), user2(user2), chatName(chatName) {}

const user &chat::getUser1() const {
    return user1;
}

const user &chat::getUser2() const {
    return user2;
}

const std::string& chat::getChatName() const {
    return chatName;
}

int chat::getTotalMessagesCount() const {
    return messages.size();
}

int chat::getUnreadMessagesCount() const {
    return std::count_if(messages.begin(), messages.end(),
                         [](const auto& msg) { return !msg->IsRead(); });
}


std::pair<int, int> chat::markMessagesAsRead(int n) {
    int count = 0;
    const int initialUnread = getUnreadMessagesCount();

    for (auto& msg : messages) {
        if (!msg->IsRead() && count < n) {
            msg->markAsRead();
            count++;
        }
    }

    return {getTotalMessagesCount(), initialUnread};
}

const std::shared_ptr<message> chat::getLastMessage() const {
    return messages.empty() ? nullptr : messages.back();
}

std::pair<int, int> chat::forEachMessage(std::function<void(const message&)> callback) const {
    int totalMessages = 0;
    int unreadMessages = 0;

    for (const auto& msg : messages) {
        totalMessages++;
        if (!msg->IsRead()) {
            unreadMessages++;
        }
        callback(*msg);
    }

    return {totalMessages, unreadMessages};
}


void chat::addMessage(const message& msg, bool isForwarded) {
    if (!isForwarded) {
        if ((msg.getSender().getId() != user1.getId() && msg.getSender().getId() != user2.getId()) ||
            (msg.getReceiver().getId() != user1.getId() && msg.getReceiver().getId() != user2.getId())) {
            throw std::invalid_argument("Errore: Uno tra il mittente o il destinatario non è un partecipante della chat.");
        }
    }
    messages.push_back(std::make_shared<message>(msg));
}

void chat::removeMessage(const message& msg) {
    if (msg.IsRead()) {
        throw MessageAlreadyReadException("Impossibile rimuovere un messaggio già letto.");
    }


    messages.remove_if([&](const std::shared_ptr<message>& m) {
        return *m == msg;
    });
}


std::list<std::shared_ptr<message>> chat::findMessageByText(const std::string& text) const {
    std::list<std::shared_ptr<message>> foundMessages;
    for (const auto& msg : messages) {
        if (msg->getText().find(text) != std::string::npos) {
            foundMessages.push_back(msg); // Ora i tipi coincidono
        }
    }
    return foundMessages;
}

void chat::forwardMessage(const message& msg, user& targetUser) {
    message forwardedMessage(
            msg.getSender(),
            targetUser,
            "Inoltrato: " + msg.getText()
    );

    addMessage(forwardedMessage, true);

    std::cout << "Messaggio inoltrato con successo da "
              << msg.getSender().getName() << " a "
              << targetUser.getName() << std::endl;
}

chat::~chat() {
    messages.clear();
}









