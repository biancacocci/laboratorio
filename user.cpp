//
// Created by Bianca Cocci on 02/03/25.
//

#include "user.h"
#include <utility>

user::user(std::string  name, int id) : name(std::move(name)) {
    if (id <= 0) {
        throw InvalidUserIdException("ID utente non valido: deve essere maggiore di zero.");
    }
    this->id = id;
}

const std::string &user::getName() const {
    return name;
}

int user::getId() const {
    return id;
}

void user::setId(int newId) {
    if (newId <= 0) {
        throw InvalidUserIdException("L'ID deve essere maggiore di zero.");
    }
    id = newId;
}

void user::setName(const std::string &name) {
    user::name = name;
}
bool user::operator==(const user& other) const {
    return id == other.id && name == other.name;
}






