#include <iostream>
#include <memory>
#include "user.h"
#include "message.h"
#include "chat.h"
#include "chatregister.h"

int main() {
    try {
        user alice("Alice", 1);
        user bob("Bob", 5);
        user carolina("Carolina", 3);

        auto chat1 = std::make_shared<chat>(alice, bob, "Chat tra Alice e Bob");
        auto chat2 = std::make_shared<chat>(alice, carolina, "Chat tra Alice e Carolina");

        chatregister registry;
        registry.addChat(chat1);
        registry.addChat(chat2);

        message msg1(alice, bob, "Ciao Bob!");
        message msg2(bob, alice, "Ciao Alice! Come stai?");
        message msg3(alice, bob, "Tutto bene, grazie!");
        message msg4(alice, bob, "Ci vediamo domani!");

        chat1->addMessage(msg1);
        chat1->addMessage(msg2);
        chat1->addMessage(msg3);
        chat1->addMessage(msg4);

        chat1->markMessagesAsRead(2);

        std::cout << "=== Stato messaggi in chat1 ===" << std::endl;
        auto [total1, unread1] = chat1->forEachMessage([&](const message& msg) {
            std::cout << msg.getSender().getName() << " -> "
                      << msg.getReceiver().getName() << ": "
                      << msg.getText()
                      << (msg.IsRead() ? " [LETTO]" : " [NON LETTO]")
                      << std::endl;
        });
        std::cout << "Messaggi totali: " << total1
                  << ", letti: " << (total1 - unread1)
                  << ", da leggere: " << unread1 << std::endl;

        try {
            chat1->removeMessage(msg1);
        } catch (const MessageAlreadyReadException& e) {
            std::cout << "\nErrore: " << e.what() << std::endl;
        }

        std::cout << "\n=== Ricerca chat ===" << std::endl;
        auto foundChat = registry.findChat(alice, bob);
        std::cout << (foundChat ? "Chat trovata: " + foundChat->getChatName()
                                : "Chat non trovata") << std::endl;

        std::cout << "\n=== Ricerca messaggi contenenti 'Ciao' ===" << std::endl;
        auto foundMessages = chat1->findMessageByText("Ciao");
        for (const auto& msg : foundMessages) {
            std::cout << msg->getSender().getName() << ": "
                      << msg->getText() << std::endl;
        }

        std::cout << "\n=== Inoltro messaggio da chat1 a chat2 ===" << std::endl;
        try {
            auto messagesToForward = chat1->findMessageByText("Tutto bene");
            if (!messagesToForward.empty()) {
                message forwardedMsg(messagesToForward.front()->getSender(),
                                     carolina,
                                     "[Inoltrato] " + messagesToForward.front()->getText());
                chat2->addMessage(forwardedMsg, true);
                std::cout << "Messaggio inoltrato con successo a Carolina" << std::endl;
            } else {
                std::cout << "Messaggio da inoltrare non trovato" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Errore nell'inoltro: " << e.what() << std::endl;
        }

        std::cout << "\n=== Messaggi in chat2 ===" << std::endl;
        auto [total2, unread2] = chat2->forEachMessage([&](const message& msg) {
            std::cout << msg.getSender().getName() << " -> "
                      << msg.getReceiver().getName() << ": "
                      << msg.getText() << std::endl;
        });
        std::cout << "Messaggi totali: " << total2
                  << ", letti: " << (total2 - unread2)
                  << ", da leggere: " << unread2 << std::endl;

        std::cout << "\n=== Chat registrate ===" << std::endl;
        registry.listChats();

    } catch (const InvalidUserIdException& e) {
        std::cerr << "Errore utente: " << e.what() << std::endl;
        return 1;
    } catch (const EmptyMessageException& e) {
        std::cerr << "Errore messaggio: " << e.what() << std::endl;
        return 2;
    } catch (const std::exception& e) {
        std::cerr << "Errore: " << e.what() << std::endl;
        return 3;
    }

    return 0;
}
