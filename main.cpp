#include <iostream>
#include "user.h"
#include "message.h"
#include "chat.h"
#include "chatregister.h"

int main() {

    try {
        // Creazione utenti
        user alice("Alice", 1);
        user bob("Bob", 5);
        user carolina("Carolina", 3);

        // Creazione chat e aggiunta al registro
        std::shared_ptr<chat> chat1=std::make_shared<chat>(alice, bob);

        chatregister registry;


        // Creazione e aggiunta messaggi
        message msg1(alice, bob, "Ciao Bob!");
        message msg2(bob, alice, "Ciao Alice! Come stai?");
        message msg3(alice, bob, "bene");

        registry.addChat(chat1);
        chat1->addMessage(msg1);
        chat1->addMessage(msg2);
        chat1->addMessage(msg3);


        // Segna i messaggi come letti
        msg1.markAsRead();
        msg2.markAsRead();

        // Stampa informazioni sui messaggi
        std::cout << "Stato dei messaggi:" << std::endl;
        for (const auto& msg : chat1->getMessages()) {
            std::cout << msg->getSender().getName() << " -> " << msg->getReceiver().getName() << ": "
                      << msg->getText();
            std::cout << (msg->getIsRead() ? " [LETTURA CONFERMATA]" : " [NON LETTO]") << std::endl;
        }

        // Tentativo di rimozione di un messaggio letto
        try {
            chat1->removeMessage(msg1);  // Dovrebbe sollevare un'eccezione
        } catch (const MessageAlreadyReadException& e) {
            std::cout << e.what() << std::endl;
        }

        // Stampa aggiornato
        std::cout << "\nMessaggi dopo il tentativo di rimozione:" << std::endl;
        for (const auto& msg : chat1->getMessages()) {
            std::cout << msg->getSender().getName() << " -> " << msg->getReceiver().getName() << ": "
                      << msg->getText();
            std::cout << (msg->getIsRead() ? " [LETTURA CONFERMATA]" : " [NON LETTO]") << std::endl;
        }

        std::cout << "\nVerifica utenti nella chat:" << std::endl;
        std::cout << (chat1->hasUser(alice) ? alice.getName() + " è nella chat!" : alice.getName() + " non è nella chat.") << std::endl;
        std::cout << (chat1->hasUser(carolina) ? carolina.getName() + " è nella chat!" : carolina.getName() + " non è nella chat.") << std::endl;


        // Ricerca chat nel registro
        std::cout << "\nVerifica registro chat:" << std::endl;
        chat* foundChat = registry.findChat(alice, bob);
        std::cout << (foundChat ? "Chat trovata tra " + foundChat->getUser1().getName() + " e " + foundChat->getUser2().getName() : "Chat non trovata tra Alice e Bob.") << std::endl;

        foundChat = registry.findChat(bob, carolina);
        std::cout << (foundChat ? "Chat trovata tra " + foundChat->getUser1().getName() + " e " + foundChat->getUser2().getName() : "Chat non trovata tra Bob e Carol.") << std::endl;

        // Test del metodo findMessageByText
        std::string searchText = "Ciao Bob!";
        message* foundMessage = chat1->findMessageByText(searchText);
        if (foundMessage) {
            std::cout << "\nMessaggio trovato: " << foundMessage->getText() << std::endl;
        } else {
            std::cout << "\nMessaggio non trovato: " << searchText << std::endl;
        }

        try {
            chat1->forwardMessage(msg1, carolina);
        } catch (const std::exception& e) {
            std::cerr << "Errore nell'inoltro del messaggio: " << e.what() << std::endl;
        }

        // Stampa i messaggi nella chat tra Alice e Carolina
        std::cout << "\nMessaggi nella chat tra Alice e Carolina:\n";
        for (const auto& msg : chat1->getMessages()) {
            if (msg->getReceiver().getName() == carolina.getName()) {
                std::cout << msg->getSender().getName() << " -> "
                          << msg->getReceiver().getName() << ": "
                          << msg->getText() << std::endl;
            }
        }
        // Elenco delle chat registrate
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
