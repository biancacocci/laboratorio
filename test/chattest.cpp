#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include "chat.h"
#include "message.h"
#include "user.h"

class ChatTest : public ::testing::Test {
protected:
    std::unique_ptr<user> user1;
    std::unique_ptr<user> user2;
    std::unique_ptr<user> user3;
    std::unique_ptr<chat> test_chat;
    std::vector<std::shared_ptr<message>> test_messages;

    void SetUp() override {
        user1 = std::make_unique<user>("Alice", 1);
        user2 = std::make_unique<user>("Bob", 2);
        user3 = std::make_unique<user>("Charlie", 3);
        test_chat = std::make_unique<chat>(*user1, *user2, "Chat Alice-Bob");

        test_messages.push_back(std::make_shared<message>(*user1, *user2, "Ciao Bob!"));
        test_messages.push_back(std::make_shared<message>(*user2, *user1, "Ciao Alice!"));
        test_messages.push_back(std::make_shared<message>(*user1, *user2, "Come stai?"));

        test_chat->addMessage(*test_messages[0]);
        test_chat->addMessage(*test_messages[1]);
    }

    void TearDown() override {
        test_messages.clear();
    }
};

TEST_F(ChatTest, Constructor) {
    EXPECT_EQ(test_chat->getUser1().getName(), "Alice");
    EXPECT_EQ(test_chat->getUser2().getName(), "Bob");
    EXPECT_EQ(test_chat->getChatName(), "Chat Alice-Bob");
    EXPECT_EQ(test_chat->getLastMessage()->getText(), "Ciao Alice!");
}

TEST_F(ChatTest, AddValidMessage) {
    EXPECT_NO_THROW(test_chat->addMessage(*test_messages[2]));
    EXPECT_EQ(test_chat->getLastMessage()->getText(), "Come stai?");
}

TEST_F(ChatTest, AddInvalidMessage) {
    message invalid_msg(*user3, *user1, "Messaggio non valido");
    EXPECT_THROW(test_chat->addMessage(invalid_msg), std::invalid_argument);
}

TEST_F(ChatTest, RemoveMessageNotRead) {
    test_messages[0]->markAsRead();
    EXPECT_THROW(test_chat->removeMessage(*test_messages[0]), MessageAlreadyReadException);

    EXPECT_NO_THROW(test_chat->removeMessage(*test_messages[1]));
    EXPECT_EQ(test_chat->getTotalMessagesCount(), 1);
}

TEST_F(ChatTest, MarkMessagesAsRead) {
    auto [total_before, unread_before] = test_chat->markMessagesAsRead(1);
    EXPECT_EQ(total_before, 2);
    EXPECT_EQ(unread_before, 2);

    EXPECT_EQ(test_chat->getUnreadMessagesCount(), 1);
}

TEST_F(ChatTest, ForEachMessage) {
    int count = 0;
    auto [total, unread] = test_chat->forEachMessage([&](const message& msg) {
        count++;
    });

    EXPECT_EQ(count, 2);
    EXPECT_EQ(total, 2);
    EXPECT_EQ(unread, 2);
}

TEST_F(ChatTest, FindMessageByText) {
    auto found1 = test_chat->findMessageByText("Ciao");
    EXPECT_EQ(found1.size(), 2);

    auto found2 = test_chat->findMessageByText("Alice");
    EXPECT_EQ(found2.size(), 1);

    auto not_found = test_chat->findMessageByText("Non esistente");
    EXPECT_TRUE(not_found.empty());
}

TEST_F(ChatTest, ForwardMessage) {
    testing::internal::CaptureStdout();
    test_chat->forwardMessage(*test_messages[0], *user3);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("Messaggio inoltrato con successo"), std::string::npos);
    EXPECT_EQ(test_chat->getTotalMessagesCount(), 3);

    bool found = false;
    test_chat->forEachMessage([&](const message& msg) {
        if (msg.getText().find("Inoltrato: Ciao Bob!") != std::string::npos) {
            found = true;
        }
    });
    EXPECT_TRUE(found);
}

TEST_F(ChatTest, GetLastMessage) {
    EXPECT_EQ(test_chat->getLastMessage()->getText(), "Ciao Alice!");

    test_chat->addMessage(*test_messages[2]);
    EXPECT_EQ(test_chat->getLastMessage()->getText(), "Come stai?");

    test_chat->removeMessage(*test_messages[1]);
    EXPECT_EQ(test_chat->getLastMessage()->getText(), "Come stai?");
}

TEST_F(ChatTest, MessageCounters) {
    test_chat->markMessagesAsRead(1);

    EXPECT_EQ(test_chat->getTotalMessagesCount(), 2);
    EXPECT_EQ(test_chat->getUnreadMessagesCount(), 1);

    test_chat->addMessage(*test_messages[2]);

    EXPECT_EQ(test_chat->getTotalMessagesCount(), 3);
    EXPECT_EQ(test_chat->getUnreadMessagesCount(), 2);
}


