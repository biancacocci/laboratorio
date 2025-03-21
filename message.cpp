//
// Created by Bianca Cocci on 02/03/25.
//
#include "message.h"
message::message(user sender, user receiver, std::string text): sender(sender) , receiver(receiver) , text(text) {
    if (text.empty()) {
        throw EmptyMessageException("Il testo del messaggio non può essere vuoto.");
    }
    this->text = text;
}


const user &message::getSender() const {
    return sender;
}

const user &message::getReceiver() const {
    return receiver;
}

const std::string &message::getText() const {
    return text;
}

void message::setText(const std::string& newText) {
    if (newText.empty()) {
        throw EmptyMessageException("Il messaggio non può essere vuoto.");
    }
    text = newText;
}


bool message::getIsRead() const {
    return isRead;
}

void message::markAsRead() {
    isRead = true;
}


