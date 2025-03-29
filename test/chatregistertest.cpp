#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include "chatregister.h"
#include "user.h"
#include "chat.h"
#include "message.h"

class ChatRegisterTest : public ::testing::Test {
protected:
    std::unique_ptr<chatregister> reg;
    std::shared_ptr<user> alice, bob, charlie;
    std::shared_ptr<chat> chatAB, chatBC;
    std::shared_ptr<message> msg1, msg2;

    void SetUp() override {
        reg = std::make_unique<chatregister>();

        alice = std::make_shared<user>("Alice", 1);
        bob = std::make_shared<user>("Bob", 2);
        charlie = std::make_shared<user>("Charlie", 3);

        chatAB = std::make_shared<chat>(*alice, *bob, "Alice-Bob");
        chatBC = std::make_shared<chat>(*bob, *charlie, "Bob-Charlie");

        msg1 = std::make_shared<message>(*alice, *bob, "Ciao Bob!");
        msg2 = std::make_shared<message>(*bob, *charlie, "Pronto Charlie?");

        chatAB->addMessage(*msg1);
        chatBC->addMessage(*msg2);
    }
};

TEST_F(ChatRegisterTest, AddChatAndVerify) {
    reg->addChat(chatAB);
    EXPECT_EQ(reg->getChats().size(), 1);

    reg->addChat(chatBC);
    EXPECT_EQ(reg->getChats().size(), 2);
}

TEST_F(ChatRegisterTest, FindExistingChat) {
    reg->addChat(chatAB);
    reg->addChat(chatBC);

    auto found1 = reg->findChat(*alice, *bob);
    ASSERT_NE(found1, nullptr);
    EXPECT_EQ(found1->getChatName(), "Alice-Bob");

    auto found2 = reg->findChat(*bob, *charlie);
    ASSERT_NE(found2, nullptr);
    EXPECT_EQ(found2->getLastMessage()->getText(), "Pronto Charlie?");
}

TEST_F(ChatRegisterTest, FindNonExistentChat) {
    EXPECT_EQ(reg->findChat(*alice, *bob), nullptr);

    reg->addChat(chatAB);
    EXPECT_EQ(reg->findChat(*alice, *charlie), nullptr);
}

TEST_F(ChatRegisterTest, RemoveChat) {
    reg->addChat(chatAB);
    reg->addChat(chatBC);

    reg->removeChat(*alice, *bob);
    EXPECT_EQ(reg->getChats().size(), 1);
    EXPECT_EQ(reg->findChat(*alice, *bob), nullptr);

    reg->removeChat(*bob, *charlie);
    EXPECT_TRUE(reg->getChats().empty());
}

TEST_F(ChatRegisterTest, GetChatsList) {
    reg->addChat(chatAB);
    reg->addChat(chatBC);

    const auto& chats = reg->getChats();
    ASSERT_EQ(chats.size(), 2);

    bool hasAB = false, hasBC = false;
    for (const auto& c : chats) {
        if (c->getChatName() == "Alice-Bob") hasAB = true;
        if (c->getChatName() == "Bob-Charlie") hasBC = true;
    }
    EXPECT_TRUE(hasAB);
    EXPECT_TRUE(hasBC);
}



TEST_F(ChatRegisterTest, ListEmptyChats) {
    testing::internal::CaptureStdout();
    reg->listChats();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("Nessuna chat registrata."), std::string::npos);
}



TEST_F(ChatRegisterTest, ChatOrderPreservation) {
    auto chatAC = std::make_shared<chat>(*alice, *charlie, "Alice-Charlie");

    reg->addChat(chatAB);
    reg->addChat(chatBC);
    reg->addChat(chatAC);

    const auto& chats = reg->getChats();
    ASSERT_EQ(chats.size(), 3);

}

TEST_F(ChatRegisterTest, FindChatWithSwappedUsers) {
    reg->addChat(chatAB);

    auto found1 = reg->findChat(*alice, *bob);
    auto found2 = reg->findChat(*bob, *alice);

    EXPECT_NE(found1, nullptr);
    EXPECT_NE(found2, nullptr);
    EXPECT_EQ(found1, found2);
}
