//
// Created by Bianca Cocci on 13/03/25.
//

#ifndef LABORATORIO_CHATREGISTER_H
#define LABORATORIO_CHATREGISTER_H
#include <list>
#include "chat.h"

class chatregister {
public:
    const std::list<std::shared_ptr<chat>> &getChats() const;

    void addChat (std::shared_ptr<chat>& chat);

    void removeChat(const user& u1, const user& u2);

    std::shared_ptr<chat> findChat(const user& u1, const user& u2);

    void listChats() const;

private:
    std::list<std::shared_ptr<chat>> chats;
};

#endif //LABORATORIO_CHATREGISTER_H
