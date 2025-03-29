#include <gtest/gtest.h>
#include <memory>
#include "message.h"
#include "user.h"

class MessageTest : public ::testing::Test {
protected:
    std::unique_ptr<user> sender;
    std::unique_ptr<user> receiver;
    std::unique_ptr<message> msg1;
    std::unique_ptr<message> msg2;

    void SetUp() override {
        sender = std::make_unique<user>("Alice", 1);
        receiver = std::make_unique<user>("Bob", 2);
        msg1 = std::make_unique<message>(*sender, *receiver, "Ciao Bob!");
        msg2 = std::make_unique<message>(*receiver, *sender, "Ciao Alice!");
    }
};

TEST_F(MessageTest, ConstructorValid) {
    EXPECT_EQ(msg1->getSender().getName(), "Alice");
    EXPECT_EQ(msg1->getReceiver().getName(), "Bob");
    EXPECT_EQ(msg1->getText(), "Ciao Bob!");
    EXPECT_FALSE(msg1->IsRead());
}

TEST_F(MessageTest, ConstructorEmptyText) {
    EXPECT_THROW(message(*sender, *receiver, ""), EmptyMessageException);
}

TEST_F(MessageTest, GetSender) {
    EXPECT_EQ(msg1->getSender().getId(), 1);
    EXPECT_EQ(msg1->getSender().getName(), "Alice");
}

TEST_F(MessageTest, GetReceiver) {
    EXPECT_EQ(msg1->getReceiver().getId(), 2);
    EXPECT_EQ(msg1->getReceiver().getName(), "Bob");
}

TEST_F(MessageTest, GetText) {
    EXPECT_EQ(msg1->getText(), "Ciao Bob!");
    EXPECT_EQ(msg2->getText(), "Ciao Alice!");
}

TEST_F(MessageTest, SetTextValid) {
    msg1->setText("Nuovo messaggio");
    EXPECT_EQ(msg1->getText(), "Nuovo messaggio");
}

TEST_F(MessageTest, SetTextEmpty) {
    EXPECT_THROW(msg1->setText(""), EmptyMessageException);
    EXPECT_EQ(msg1->getText(), "Ciao Bob!"); // Verifica che il testo originale non sia cambiato
}

TEST_F(MessageTest, IsReadInitiallyFalse) {
    EXPECT_FALSE(msg1->IsRead());
    EXPECT_FALSE(msg2->IsRead());
}

TEST_F(MessageTest, MarkAsRead) {
    EXPECT_FALSE(msg1->IsRead());
    msg1->markAsRead();
    EXPECT_TRUE(msg1->IsRead());


    msg1->markAsRead();
    EXPECT_TRUE(msg1->IsRead());
}

TEST_F(MessageTest, EqualityOperator) {
    message msg3(*sender, *receiver, "Ciao Bob!");
    message msg4(*sender, *receiver, "Ciao Bob!");
    msg4.markAsRead();


    EXPECT_TRUE(*msg1 == msg3);


    EXPECT_FALSE(*msg1 == msg4);


    EXPECT_FALSE(*msg1 == *msg2);


    message msg5(*receiver, *sender, "Ciao Bob!");
    EXPECT_FALSE(*msg1 == msg5);
}

TEST_F(MessageTest, CopyConstructor) {
    message msg_copy(*msg1);
    EXPECT_EQ(msg_copy.getText(), msg1->getText());
    EXPECT_EQ(msg_copy.getSender().getName(), msg1->getSender().getName());
    EXPECT_EQ(msg_copy.getReceiver().getName(), msg1->getReceiver().getName());
    EXPECT_EQ(msg_copy.IsRead(), msg1->IsRead());
}

TEST_F(MessageTest, AssignmentOperator) {
    message msg3(*receiver, *sender, "Temp");
    msg3 = *msg1;

    EXPECT_EQ(msg3.getText(), msg1->getText());
    EXPECT_EQ(msg3.getSender().getName(), msg1->getSender().getName());
    EXPECT_EQ(msg3.getReceiver().getName(), msg1->getReceiver().getName());
    EXPECT_EQ(msg3.IsRead(), msg1->IsRead());
}

