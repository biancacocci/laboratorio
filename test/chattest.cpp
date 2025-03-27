#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include "chat.h"

class ChatTest : public ::testing::Test {
protected:
    std::unique_ptr<user> user1;
    std::unique_ptr<user> user2;
    std::unique_ptr<user> user3;
    std::unique_ptr<chat> chat1;
    std::vector<std::shared_ptr<message>> messages;

    void SetUp() override {
        user1 = std::make_unique<user>("Alice", 1);
        user2 = std::make_unique<user>("Bob", 2);
        user3 = std::make_unique<user>("Charlie", 3);
        chat1 = std::make_unique<chat>(*user1, *user2);
    }
};

TEST_F(ChatTest, Constructor) {
    EXPECT_EQ(chat1->getUser1(), *user1);
    EXPECT_EQ(chat1->getUser2(), *user2);
    EXPECT_EQ(chat1->getLastMessage(), nullptr);
}

TEST_F(ChatTest, AddMessage) {
    messages.push_back(std::make_shared<message>(*user1, *user2, "Ciao Bob!"));
    chat1->addMessage(*messages.back());

    int count = 0;
    chat1->forEachMessage([&](const message& msg) { count++; });
    EXPECT_EQ(count, 1);
    EXPECT_EQ(chat1->getLastMessage()->getText(), "Ciao Bob!");
}

TEST_F(ChatTest, RemoveMessageNotRead) {
    messages.push_back(std::make_shared<message>(*user1, *user2, "Messaggio da eliminare"));
    chat1->addMessage(*messages.back());

    chat1->removeMessage(*messages.back());
    EXPECT_EQ(chat1->getLastMessage(), nullptr);
}

TEST_F(ChatTest, RemoveMessageAlreadyRead) {
    messages.push_back(std::make_shared<message>(*user1, *user2, "Messaggio già letto"));
    messages.back()->markAsRead();
    chat1->addMessage(*messages.back());

    EXPECT_THROW(chat1->removeMessage(*messages.back()), MessageAlreadyReadException);
}



TEST_F(ChatTest, FindMessageByText_Exists) {
    messages.push_back(std::make_shared<message>(*user1, *user2, "Ciao Bob!"));
    chat1->addMessage(*messages.back());

    auto result = chat1->findMessageByText("Ciao Bob!");
    ASSERT_FALSE(result.empty());
    EXPECT_EQ(result.front()->getText(), "Ciao Bob!");
}

TEST_F(ChatTest, FindMessageByText_NotExists) {
    auto result = chat1->findMessageByText("Messaggio inesistente");
    EXPECT_TRUE(result.empty());
}

TEST_F(ChatTest, ForwardMessageValid) {
    messages.push_back(std::make_shared<message>(*user1, *user2, "Messaggio originale"));
    chat1->addMessage(*messages.back());

    chat1->forwardMessage(*messages.back(), *user3);

    bool found = false;
    chat1->forEachMessage([&](const message& msg) {
        if(msg.getText() == "Inoltrato: Messaggio originale") found = true;
    });
    EXPECT_TRUE(found);
}

TEST_F(ChatTest, ForwardMessageInvalid) {
    message fakeMessage(*user1, *user2, "Messaggio falso");
    EXPECT_THROW(chat1->forwardMessage(fakeMessage, *user3), std::invalid_argument);
}

