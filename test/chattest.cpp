#include <gtest/gtest.h>
#include "chat.h"

class ChatTest : public ::testing::Test {
protected:
    user user1, user2, user3;
    chat* chat1;
    std::vector<message> messages;  // Mantiene i messaggi in scope

    void SetUp() override {
        user1 = user("Alice", 1);
        user2 = user("Bob", 2);
        user3 = user("Charlie", 3);
        chat1 = new chat(user1, user2);
    }

    void TearDown() override {
        delete chat1;
    }
};

TEST_F(ChatTest, Constructor) {
    EXPECT_EQ(chat1->getUser1(), user1);
    EXPECT_EQ(chat1->getUser2(), user2);
    EXPECT_EQ(chat1->getLastMessage(), nullptr);  // Sostituisce getMessages().empty()
}

TEST_F(ChatTest, AddMessage) {
    messages.emplace_back(user1, user2, "Ciao Bob!");
    chat1->addMessage(messages.back());

    int count = 0;
    chat1->forEachMessage([&](const message& msg) { count++; });  // Conta i messaggi
    EXPECT_EQ(count, 1);
    EXPECT_EQ(chat1->getLastMessage()->getText(), "Ciao Bob!");
}

TEST_F(ChatTest, RemoveMessageNotRead) {
    messages.emplace_back(user1, user2, "Messaggio da eliminare");
    chat1->addMessage(messages.back());

    chat1->removeMessage(messages.back());
    EXPECT_EQ(chat1->getLastMessage(), nullptr);  // Verifica che non ci siano messaggi
}

TEST_F(ChatTest, RemoveMessageAlreadyRead) {
    messages.emplace_back(user1, user2, "Messaggio già letto");
    messages.back().markAsRead();
    chat1->addMessage(messages.back());

    EXPECT_THROW(chat1->removeMessage(messages.back()), MessageAlreadyReadException);
}

TEST_F(ChatTest, HasUser) {
    EXPECT_TRUE(chat1->hasUser(user1));
    EXPECT_TRUE(chat1->hasUser(user2));
    EXPECT_FALSE(chat1->hasUser(user3));
}

TEST_F(ChatTest, FindMessageByText_Exists) {
    messages.emplace_back(user1, user2, "Ciao Bob!");
    chat1->addMessage(messages.back());

    auto result = chat1->findMessageByText("Ciao Bob!");
    ASSERT_FALSE(result.empty());
    EXPECT_EQ(result.front()->getText(), "Ciao Bob!");
}

TEST_F(ChatTest, FindMessageByText_NotExists) {
    auto result = chat1->findMessageByText("Messaggio inesistente");
    EXPECT_TRUE(result.empty());
}

TEST_F(ChatTest, ForwardMessageValid) {
    messages.emplace_back(user1, user2, "Messaggio originale");
    chat1->addMessage(messages.back());

    chat1->forwardMessage(messages.back(), user3);

    bool found = false;
    chat1->forEachMessage([&](const message& msg) {  // Cerca il messaggio inoltrato
        if(msg.getText() == "Inoltrato: Messaggio originale") found = true;
    });
    EXPECT_TRUE(found);
}

TEST_F(ChatTest, ForwardMessageInvalid) {
    message fakeMessage(user1, user2, "Messaggio falso");
    EXPECT_THROW(chat1->forwardMessage(fakeMessage, user3), std::invalid_argument);
}

