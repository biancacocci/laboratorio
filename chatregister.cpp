//
// Created by Bianca Cocci on 13/03/25.
//

#include "chatregister.h"
#include <iostream>
chatregister::chatregister() {}

const std::list<std::shared_ptr<chat>> &chatregister::getChats() const {
    return chats;
}

void chatregister:: addChat(const std::shared_ptr<chat>& chat) {
    chats.push_front(chat);
}

void chatregister::removeChat(const user& u1, const user& u2) {
    chats.remove_if([&](const std::shared_ptr<chat>& c) {
        return (c->getUser1() == u1 && c->getUser2() == u2) ||
               (c->getUser1() == u2 && c->getUser2() == u1);
    });
}

chat* chatregister::findChat(const user& u1, const user& u2) {
    for (auto& c : chats) {
        if ((c->getUser1() == u1 && c->getUser2() == u2) ||
            (c->getUser1() == u2 && c->getUser2() == u1)) {
            return c.get();
        }
    }
    return nullptr;
}

void chatregister::listChats() const {
    if (chats.empty()) {
        std::cout << "Nessuna chat registrata.\n";
        return;
    }

    std::cout << "Registro chat:\n";
    int index = 1;
    for (const auto& c : chats) {
        std::cout << index++ << ". " << c->getUser1().getName() << " & " << c->getUser2().getName() << "\n";

        // Mostriamo solo una breve anteprima dell'ultimo messaggio, se esiste
        const auto& messages = c->getMessages();
        if (!messages.empty()) {
            const message* lastMessage = messages.back();  // Puntatore all'ultimo messaggio
            std::cout << "   Ultimo messaggio: "
                      << lastMessage->getSender().getName() << " ha detto: '"
                      << lastMessage->getText() << "'\n";
        } else {
            std::cout << "   Nessun messaggio ancora.\n";
        }
    }
}






