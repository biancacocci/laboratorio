#include <gtest/gtest.h>
#include <memory>
#include "chatregister.h"
#include "user.h"
#include "chat.h"

class ChatRegisterTest : public ::testing::Test {
protected:
    std::unique_ptr<chatregister> chatReg;
    std::shared_ptr<user> user1, user2, user3;
    std::shared_ptr<chat> chat1, chat2;
    std::shared_ptr<message> msg1, msg2;

    void SetUp() override {
        chatReg = std::make_unique<chatregister>();

        user1 = std::make_shared<user>("Alice", 1);
        user2 = std::make_shared<user>("Bob", 2);
        user3 = std::make_shared<user>("Charlie", 3);

        chat1 = std::make_shared<chat>(*user1, *user2);
        chat2 = std::make_shared<chat>(*user2, *user3);

        msg1 = std::make_shared<message>(*user1, *user2, "Ciao Bob!");
        msg2 = std::make_shared<message>(*user2, *user3, "Pronto Charlie!");

        chat1->addMessage(*msg1);
        chat2->addMessage(*msg2);

        chatReg->addChat(chat1);
        chatReg->addChat(chat2);
    }
};

TEST_F(ChatRegisterTest, AddChat) {
    EXPECT_NE(chatReg->findChat(*user1, *user2), nullptr);
    EXPECT_NE(chatReg->findChat(*user2, *user3), nullptr);
}

TEST_F(ChatRegisterTest, RemoveChat) {
    chatReg->removeChat(*user1, *user2);
    EXPECT_EQ(chatReg->findChat(*user1, *user2), nullptr);
    EXPECT_NE(chatReg->findChat(*user2, *user3), nullptr);
}

TEST_F(ChatRegisterTest, FindChat) {
    auto foundChat = chatReg->findChat(*user1, *user2);
    ASSERT_NE(foundChat, nullptr);
    EXPECT_EQ(foundChat->getUser1().getName(), "Alice");
    EXPECT_EQ(foundChat->getUser2().getName(), "Bob");
    EXPECT_EQ(chatReg->findChat(*user1, *user3), nullptr);
}

TEST_F(ChatRegisterTest, GetChats) {
    const auto& chatList = chatReg->getChats();
    EXPECT_EQ(chatList.size(), 2);

    bool found1 = false, found2 = false;
    for (const auto& c : chatList) {
        if (c->getUser1().getName() == "Alice" && c->getUser2().getName() == "Bob")
            found1 = true;
        if (c->getUser1().getName() == "Bob" && c->getUser2().getName() == "Charlie")
            found2 = true;
    }
    EXPECT_TRUE(found1);
    EXPECT_TRUE(found2);
}
TEST_F(ChatRegisterTest, ListChats) {
    testing::internal::CaptureStdout();
    chatReg->listChats();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("Registro chat:"), std::string::npos);
    EXPECT_NE(output.find("Alice & Bob"), std::string::npos);
    EXPECT_NE(output.find("Bob & Charlie"), std::string::npos);
    EXPECT_NE(output.find("Ciao Bob!"), std::string::npos);
    EXPECT_NE(output.find("Pronto Charlie!"), std::string::npos);
    EXPECT_NE(output.find("Ultimo messaggio:"), std::string::npos);
}



