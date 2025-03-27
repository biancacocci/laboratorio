#include <gtest/gtest.h>
#include <memory>
#include "message.h"
#include "user.h"

class MessageTest : public ::testing::Test {
protected:
    std::unique_ptr<user> sender;
    std::unique_ptr<user> receiver;
    std::unique_ptr<message> test_msg;
    std::unique_ptr<message> empty_msg;

    void SetUp() override {
        sender = std::make_unique<user>("Alice", 1);
        receiver = std::make_unique<user>("Bob", 2);
        test_msg = std::make_unique<message>(*sender, *receiver, "Ciao Bob!");
    }
};

TEST_F(MessageTest, Constructor) {
    EXPECT_EQ(test_msg->getSender().getName(), "Alice");
    EXPECT_EQ(test_msg->getReceiver().getName(), "Bob");
    EXPECT_EQ(test_msg->getText(), "Ciao Bob!");
    EXPECT_FALSE(test_msg->getIsRead());
}

TEST_F(MessageTest, ConstructorEmptyText) {
    EXPECT_THROW(message(*sender, *receiver, ""), EmptyMessageException);
}

TEST_F(MessageTest, GetSender) {
    EXPECT_EQ(test_msg->getSender().getId(), 1);
    EXPECT_EQ(test_msg->getSender().getName(), "Alice");
}

TEST_F(MessageTest, GetReceiver) {
    EXPECT_EQ(test_msg->getReceiver().getId(), 2);
    EXPECT_EQ(test_msg->getReceiver().getName(), "Bob");
}

TEST_F(MessageTest, GetText) {
    EXPECT_EQ(test_msg->getText(), "Ciao Bob!");
}

TEST_F(MessageTest, SetTextValid) {
    test_msg->setText("Nuovo messaggio");
    EXPECT_EQ(test_msg->getText(), "Nuovo messaggio");
}

TEST_F(MessageTest, SetTextEmpty) {
    EXPECT_THROW(test_msg->setText(""), EmptyMessageException);
}

TEST_F(MessageTest, GetIsReadInitiallyFalse) {
    EXPECT_FALSE(test_msg->getIsRead());
}

TEST_F(MessageTest, MarkAsRead) {
    test_msg->markAsRead();
    EXPECT_TRUE(test_msg->getIsRead());


    test_msg->markAsRead();
    EXPECT_TRUE(test_msg->getIsRead());
}

TEST_F(MessageTest, EqualityOperator) {

    message msg1(*sender, *receiver, "Ciao Bob!");
    message msg2(*sender, *receiver, "Ciao Bob!");


    message msg3(*receiver, *sender, "Ciao Alice!");
    message msg4(*sender, *receiver, "Ciao Bob!");
    msg4.markAsRead();


    EXPECT_TRUE(msg1 == msg2);
    EXPECT_FALSE(msg1 == msg3);
    EXPECT_FALSE(msg1 == msg4);
}

TEST_F(MessageTest, CopyConstructor) {
    message msg_copy(*test_msg);
    EXPECT_EQ(msg_copy.getText(), test_msg->getText());
    EXPECT_EQ(msg_copy.getSender().getName(), test_msg->getSender().getName());
    EXPECT_EQ(msg_copy.getIsRead(), test_msg->getIsRead());
}

