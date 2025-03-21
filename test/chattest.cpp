#include <gtest/gtest.h>
#include "chat.h"

class ChatTest : public ::testing::Test {
protected:
    // Oggetti necessari per i test
    user user1, user2, user3;
    chat* chat1;

    // Setup che viene eseguito prima di ogni test
    void SetUp() override {
        user1 = user("Alice", 1);
        user2 = user("Bob", 2);
        user3 = user("Charlie", 3);

        // Creazione della chat tra user1 e user2
        chat1 = new chat(user1, user2);
    }

    // Destructor che si occupa di deallocare la chat creata
    void TearDown() override {
        delete chat1;  // Libera la memoria della chat
    }
};

// Test del costruttore
TEST_F(ChatTest, Constructor) {
    EXPECT_EQ(chat1->getUser1(), user1);
    EXPECT_EQ(chat1->getUser2(), user2);
    EXPECT_TRUE(chat1->getMessages().empty());
}

// Test di `addMessage()`
TEST_F(ChatTest, AddMessage) {
    message m(user1, user2, "Ciao Bob!");
    chat1->addMessage(m);

    ASSERT_EQ(chat1->getMessages().size(), 1);
    EXPECT_EQ(chat1->getMessages().front()->getText(), "Ciao Bob!");
}

// Test di `removeMessage()` con messaggio non letto
TEST_F(ChatTest, RemoveMessageNotRead) {
    message m(user1, user2, "Messaggio da eliminare");
    chat1->addMessage(m);

    chat1->removeMessage(m);
    EXPECT_TRUE(chat1->getMessages().empty());
}

// Test di `removeMessage()` con messaggio già letto
TEST_F(ChatTest, RemoveMessageAlreadyRead) {
    message m(user1, user2, "Messaggio già letto");
    m.markAsRead();
    chat1->addMessage(m);

    EXPECT_THROW(chat1->removeMessage(m), MessageAlreadyReadException);
}

// Test di `hasUser()` per utente presente e assente
TEST_F(ChatTest, HasUser) {
    EXPECT_TRUE(chat1->hasUser(user1));
    EXPECT_TRUE(chat1->hasUser(user2));
    EXPECT_FALSE(chat1->hasUser(user3));
}

// Test di `findMessageByText()` quando il messaggio esiste
TEST_F(ChatTest, FindMessageByText_Exists) {
    message m(user1, user2, "Ciao Bob!");
    chat1->addMessage(m);

    EXPECT_EQ(chat1->findMessageByText("Ciao Bob!"), &m);
}

// Test di `findMessageByText()` quando il messaggio non esiste
TEST_F(ChatTest, FindMessageByText_NotExists) {
    EXPECT_EQ(chat1->findMessageByText("Messaggio inesistente"), nullptr);
}

// Test di `forwardMessage()` con messaggio esistente
TEST_F(ChatTest, ForwardMessageValid) {
    message m(user1, user2, "Messaggio originale");
    chat1->addMessage(m);

    chat1->forwardMessage(m, user3);

    EXPECT_EQ(chat1->getMessages().back()->getText(), "Inoltrato: Messaggio originale");
}

// Test di `forwardMessage()` con messaggio non esistente
TEST_F(ChatTest, ForwardMessageInvalid) {
    message fakeMessage(user1, user2, "Messaggio falso");

    EXPECT_THROW(chat1->forwardMessage(fakeMessage, user3), std::invalid_argument);
}

