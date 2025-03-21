#include "user.h"
#include "chat.h"
#include <iostream>
// Costruttore della chat
chat::chat(user& user1, user& user2): user1(user1), user2(user2) {}

// Getter per user1
const user &chat::getUser1() const {
    return user1;
}

// Getter per user2
const user &chat::getUser2() const {
    return user2;
}

// Getter per i messaggi
const std::list<message*> &chat::getMessages() const {
    return messages;
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

message* chat::findMessageByText(const std::string& text) const {
    for (auto& msg : messages) {
        if (msg->getText() == text) {
            return msg;
        }
    }
    return nullptr;  // Restituisce nullptr se il messaggio non viene trovato
}

void chat::forwardMessage(const message& msg, user& targetUser) {
    // Controlliamo se il messaggio esiste
    if (findMessageByText(msg.getText()) == nullptr) {
        throw std::invalid_argument("Il messaggio specificato non esiste nella chat.");
    }

    // Crea il nuovo messaggio con il prefisso "Inoltrato: "
    message* forwardedMessage = new message(
            msg.getSender(), // Il mittente rimane invariato
            targetUser, // Il destinatario della chat di destinazione
            "Inoltrato: " + msg.getText() // Aggiungi il tag "Inoltrato: "
    );

    // Aggiungiamo il messaggio alla chat della destinazione
    addMessage(*forwardedMessage);

    std::cout << "Messaggio inoltrato con successo da " << msg.getSender().getName() << " a " << targetUser.getName() << std::endl;
}



chat::~chat() {
    for (auto& msg : messages) {}
    messages.clear(); // Pulisce la lista dei messaggi
}









