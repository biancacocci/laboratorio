#include <gtest/gtest.h>
#include "message.h"

class MessageTest : public ::testing::Test {
protected:
    // Oggetti necessari per i test
    user sender, receiver;
    message* msg;

    // Setup che viene eseguito prima di ogni test
    void SetUp() override {
        sender = user("Alice", 1);
        receiver = user("Bob", 2);

        // Creazione del messaggio
        msg = new message(sender, receiver, "Ciao Bob!");
    }

    // Destructor che si occupa di deallocare il messaggio creato
    void TearDown() override {
        delete msg;  // Libera la memoria del messaggio
    }
};

// Test del costruttore
TEST_F(MessageTest, Constructor) {
    EXPECT_EQ(msg->getSender(), sender);
    EXPECT_EQ(msg->getReceiver(), receiver);
    EXPECT_EQ(msg->getText(), "Ciao Bob!");
}

// Test di un costruttore che lancia un'eccezione quando il testo è vuoto
TEST_F(MessageTest, Constructor_EmptyText) {
    EXPECT_THROW(new message(sender, receiver, ""), EmptyMessageException);
}

// Test di `getSender()`
TEST_F(MessageTest, GetSender) {
    EXPECT_EQ(msg->getSender(), sender);
}

// Test di `getReceiver()`
TEST_F(MessageTest, GetReceiver) {
    EXPECT_EQ(msg->getReceiver(), receiver);
}

// Test di `getText()`
TEST_F(MessageTest, GetText) {
    EXPECT_EQ(msg->getText(), "Ciao Bob!");
}

// Test di `setText()` con testo valido
TEST_F(MessageTest, SetText_Valid) {
    msg->setText("Nuovo messaggio");
    EXPECT_EQ(msg->getText(), "Nuovo messaggio");
}

// Test di `setText()` con testo vuoto (lancia un'eccezione)
TEST_F(MessageTest, SetText_Empty) {
    EXPECT_THROW(msg->setText(""), EmptyMessageException);
}

// Test di `getIsRead()` (inizialmente non letto)
TEST_F(MessageTest, GetIsRead) {
    EXPECT_FALSE(msg->getIsRead());
}

// Test di `markAsRead()` (dopo la lettura)
TEST_F(MessageTest, MarkAsRead) {
    msg->markAsRead();
    EXPECT_TRUE(msg->getIsRead());
}