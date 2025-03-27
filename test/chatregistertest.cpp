#include <gtest/gtest.h>
#include <memory>
#include "chatregister.h"
#include "user.h"
#include "chat.h"

class ChatRegisterTest : public ::testing::Test {
protected:
    chatregister chatReg;
    user user1, user2, user3;
    std::shared_ptr<chat> chat1, chat2;
    message msg1, msg2;

    void SetUp() override {
        user1 = user("Alice", 1);
        user2 = user("Bob", 2);
        user3 = user("Charlie", 3);

        chat1 = std::make_shared<chat>(user1, user2);
        chat2 = std::make_shared<chat>(user2, user3);

        // Aggiunge messaggi di test
        msg1 = message(user1, user2, "Ciao Bob!");
        msg2 = message(user2, user3, "Pronto Charlie!");
        chat1->addMessage(msg1);
        chat2->addMessage(msg2);

        chatReg.addChat(chat1);
        chatReg.addChat(chat2);
    }
};

TEST_F(ChatRegisterTest, AddChat) {
    // Verifica indiretta tramite findChat
    EXPECT_NE(chatReg.findChat(user1, user2), nullptr);
    EXPECT_NE(chatReg.findChat(user2, user3), nullptr);
}

TEST_F(ChatRegisterTest, RemoveChat) {
    chatReg.removeChat(user1, user2);

    // Verifica che la chat sia stata rimossa
    EXPECT_EQ(chatReg.findChat(user1, user2), nullptr);

    // Verifica che l'altra chat esista ancora
    EXPECT_NE(chatReg.findChat(user2, user3), nullptr);
}

TEST_F(ChatRegisterTest, FindChat) {
    // Verifica chat esistente
    chat* foundChat = chatReg.findChat(user1, user2);
    ASSERT_NE(foundChat, nullptr);
    EXPECT_EQ(foundChat->getUser1().getName(), "Alice");
    EXPECT_EQ(foundChat->getUser2().getName(), "Bob");

    // Verifica chat non esistente
    EXPECT_EQ(chatReg.findChat(user1, user3), nullptr);
}

TEST_F(ChatRegisterTest, ListChats) {
    testing::internal::CaptureStdout();
    chatReg.listChats();
    std::string output = testing::internal::GetCapturedStdout();

    // Verifica struttura base
    EXPECT_NE(output.find("Registro chat:"), std::string::npos);

    // Verifica presenza chat
    EXPECT_NE(output.find("Alice & Bob"), std::string::npos);
    EXPECT_NE(output.find("Bob & Charlie"), std::string::npos);

    // Verifica ultimi messaggi
    EXPECT_NE(output.find("Ciao Bob!"), std::string::npos);
    EXPECT_NE(output.find("Pronto Charlie!"), std::string::npos);

    // Verifica formattazione
    EXPECT_NE(output.find("Ultimo messaggio:"), std::string::npos);
}

