#include <iostream>
#include "user.h"
#include "message.h"
#include "chat.h"
#include "chatregister.h"

int main() {

    try {

        user alice("Alice", 1);
        user bob("Bob", 5);
        user carolina("Carolina", 3);


        std::shared_ptr<chat> chat1=std::make_shared<chat>(alice, bob);

        chatregister registry;


        message msg1(alice, bob, "Ciao Bob!");
        message msg2(bob, alice, "Ciao Alice! Come stai?");
        message msg3(alice, bob, "bene");
        message msg4(alice, bob, "Ciao Bob, ci vediamo dopo!");

        registry.addChat(chat1);
        chat1->addMessage(msg1);
        chat1->addMessage(msg2);
        chat1->addMessage(msg3);
        chat1->addMessage(msg4);


        msg1.markAsRead();
        msg2.markAsRead();


        std::cout << "Stato dei messaggi:" << std::endl;
        chat1->forEachMessage([&](const message& msg) {
            std::cout << msg.getSender().getName() << " -> " << msg.getReceiver().getName() << ": "
                      << msg.getText();
            std::cout << (msg.getIsRead() ? " [LETTURA CONFERMATA]" : " [NON LETTO]") << std::endl;
        });


        try {
            chat1->removeMessage(msg1);
        } catch (const MessageAlreadyReadException& e) {
            std::cout << e.what() << std::endl;
        }


        std::cout << "\nMessaggi dopo il tentativo di rimozione:" << std::endl;
        chat1->forEachMessage([&](const message& msg) {
            std::cout << msg.getSender().getName() << " -> " << msg.getReceiver().getName() << ": "
                      << msg.getText();
            std::cout << (msg.getIsRead() ? " [LETTURA CONFERMATA]" : " [NON LETTO]") << std::endl;
        });




        std::cout << "\nVerifica registro chat:" << std::endl;
        chat* foundChat = registry.findChat(alice, bob);
        std::cout << (foundChat ? "Chat trovata tra " + foundChat->getUser1().getName() + " e " + foundChat->getUser2().getName() : "Chat non trovata tra Alice e Bob.") << std::endl;

        foundChat = registry.findChat(bob, carolina);
        std::cout << (foundChat ? "Chat trovata tra " + foundChat->getUser1().getName() + " e " + foundChat->getUser2().getName() : "Chat non trovata tra Bob e Carol.") << std::endl;


        std::string searchText = "Ciao Bob";
        auto foundMessages = chat1->findMessageByText(searchText);
        if (!foundMessages.empty()) {
            std::cout << "\nMessaggi trovati:" << std::endl;
            for (const auto& msg : foundMessages) {
                std::cout << msg->getSender().getName() << " -> "
                          << msg->getReceiver().getName() << ": "
                          << msg->getText() << std::endl;
            }
        } else {
            std::cout << "\nMessaggio non trovato: " << searchText << std::endl;
        }

        try {
            chat1->forwardMessage(msg1, carolina);
        } catch (const std::exception& e) {
            std::cerr << "Errore nell'inoltro del messaggio: " << e.what() << std::endl;
        }


        std::cout << "\nMessaggi nella chat tra Alice e Carolina:\n";
        chat1->forEachMessage([&](const message& msg) {
            if (msg.getReceiver().getName() == carolina.getName()) {
                std::cout << msg.getSender().getName() << " -> "
                          << msg.getReceiver().getName() << ": "
                          << msg.getText() << std::endl;
            }
        });


        std::cout << "\nElenco delle chat registrate:\n";
        registry.listChats();

    } catch (const InvalidUserIdException& e) {
        std::cerr << "Errore nella creazione dell'utente: " << e.what() << std::endl;
        return 1;
    } catch (const EmptyMessageException& e) {
        std::cerr << "Errore nella creazione del messaggio: " << e.what() << std::endl;
        return 2;
    }

    return 0;
}