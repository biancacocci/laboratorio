
#include <gtest/gtest.h>
#include <memory>
#include "chatregister.h"
#include "user.h"  // Assumiamo che la classe User sia definita separatamente
#include "chat.h"   // Assumiamo che la classe Chat sia definita separatamente


class ChatRegisterTest : public ::testing::Test {
protected:
    chatregister chatReg;
    user user1, user2, user3;
    std::shared_ptr<chat> chat1, chat2;

    void SetUp() override {

        user1 = user("Alice", 1);
        user2 = user("Bob", 2);
        user3 = user("Charlie", 3);

        // Setup di base per le chat
        chat1 = std::make_shared<chat>(user1,user2);
        chat2 = std::make_shared<chat>(user2, user3);

        // Aggiungo le chat al registro
        chatReg.addChat(chat1);
        chatReg.addChat(chat2);

    }

};

TEST_F(ChatRegisterTest, AddChat) {
    // Verifica che le chat siano state aggiunte correttamente
    const auto& chats = chatReg.getChats();
    ASSERT_EQ(chats.size(), 2);  // Dovrebbero esserci 2 chat
}

TEST_F(ChatRegisterTest, RemoveChat) {
    // Rimuovo una chat
    chatReg.removeChat(user1, user2);

    // Verifica che la chat sia stata rimossa
    const auto& chats = chatReg.getChats();
    ASSERT_EQ(chats.size(), 1);  // Dovrebbe restare solo una chat
    ASSERT_EQ(chats.front()->getUser1().getName(), "Bob");  // La chat rimanente dovrebbe essere quella tra Bob e Charlie
}

TEST_F(ChatRegisterTest, FindChat) {
    // Verifica che findChat restituisca correttamente la chat
    chat* foundChat = chatReg.findChat(user1, user2);
    ASSERT_NE(foundChat, nullptr);  // Dovrebbe trovare la chat tra Alice e Bob
    ASSERT_EQ(foundChat->getUser1().getName(), "Alice");
    ASSERT_EQ(foundChat->getUser2().getName(), "Bob");

    // Verifica che findChat restituisca nullptr per una chat che non esiste
    chat* notFoundChat = chatReg.findChat(user1, user3);
    ASSERT_EQ(notFoundChat, nullptr);  // Non dovrebbe trovare la chat tra Alice e Charlie
}

TEST_F(ChatRegisterTest, ListChats) {
    // Simuliamo l'output del metodo listChats
    testing::internal::CaptureStdout();
    chatReg.listChats();
    std::string output = testing::internal::GetCapturedStdout();

    ASSERT_NE(output.find("Registro chat:"), std::string::npos);  // Dovrebbe contenere "Registro chat"
    ASSERT_NE(output.find("Alice & Bob"), std::string::npos);      // Dovrebbe contenere il nome di Alice e Bob
    ASSERT_NE(output.find("Bob & Charlie"), std::string::npos);    // Dovrebbe contenere il nome di Bob e Charlie
}

