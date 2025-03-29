//
// Created by Bianca Cocci on 02/03/25.
//
#include "message.h"

message::message(const user& sender, const user& receiver, const std::string& text): sender(sender) , receiver(receiver) , text(text) {
    if (text.empty()) {
        throw EmptyMessageException("Il testo del messaggio non può essere vuoto.");
    }
    this->text = text;
}

bool message::operator==(const message& other) const {
    return (sender == other.sender) &&
           (receiver == other.receiver) &&
           (text == other.text) &&
           (isRead == other.isRead);
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

bool message::IsRead() const {
    return isRead;
}

void message::markAsRead() {
    isRead = true;
}


