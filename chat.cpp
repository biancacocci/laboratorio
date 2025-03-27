#include "user.h"
#include "chat.h"
#include <iostream>
// Costruttore della chat
chat::chat(const user& user1, const  user& user2): user1(user1), user2(user2) {}

// Getter per user1
const user &chat::getUser1() const {
    return user1;
}

// Getter per user2
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


// Aggiunge un messaggio alla chat (passa un puntatore al messaggio)
void chat::addMessage( message &msg) {
    messages.push_back(&msg);  // Aggiungi il puntatore al messaggio
}

// Rimuove un messaggio dalla chat (confronta i puntatori)
void chat::removeMessage(const message& msg) {
    // Controlla se il messaggio è stato letto
    if (msg.getIsRead()) {
        throw MessageAlreadyReadException("Impossibile rimuovere un messaggio già letto.");
    }

    // Se il messaggio non è stato letto, lo rimuoviamo
    messages.remove_if([&](const message* m) {
        return m->getSender() == msg.getSender() && m->getReceiver() == msg.getReceiver() && m->getText() == msg.getText();
    });
}

// Controlla se un utente è presente nella chat
bool chat::hasUser(const user &u) const {
    return u == user1 || u == user2;
}

std::list<message*> chat::findMessageByText(const std::string& text) const {
    std::list<message*> foundMessages;
    for (auto& msg : messages) {
        // Usa find() per cercare la sottostringa nel messaggio
        if (msg->getText().find(text) != std::string::npos) {
            foundMessages.push_back(msg);  // Aggiungi il messaggio alla lista se il testo è trovato
        }
    }
    return foundMessages;  // Restituisce la lista dei messaggi trovati
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
    for (auto& msg : messages) {}
    messages.clear(); // Pulisce la lista dei messaggi
}









