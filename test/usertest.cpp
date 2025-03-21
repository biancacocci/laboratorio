#include <gtest/gtest.h>
#include "user.h"

// Fixture per User
class UserTest : public ::testing::Test {
protected:
    // Variabili per i test
    user* u1;
    user* u2;
    user* u3;

    // Funzione di setup chiamata prima di ogni test
    void SetUp() override {
        // Creazione degli oggetti user
        u1 = new user("Alice", 1);
        u2 = new user("Alice", 1);
        u3 = new user("Bob", 2);
    }

    // Funzione di teardown chiamata dopo ogni test
    void TearDown() override {
        // Pulizia della memoria
        delete u1;
        delete u2;
        delete u3;
    }
};

// Test del costruttore con valori validi
TEST_F(UserTest, ConstructorValid) {
    EXPECT_EQ(u1->getName(), "Alice");
    EXPECT_EQ(u1->getId(), 1);
}

// Test del costruttore con ID non valido
TEST_F(UserTest, ConstructorInvalidId) {
    EXPECT_THROW(user("Alice", -1), InvalidUserIdException);
    EXPECT_THROW(user("Bob", 0), InvalidUserIdException);
}

// Test del metodo `setId()` con valore valido
TEST_F(UserTest, SetIdValid) {
    u1->setId(10);
    EXPECT_EQ(u1->getId(), 10);
}

// Test del metodo `setId()` con valore non valido
TEST_F(UserTest, SetIdInvalid) {
    EXPECT_THROW(u1->setId(0), InvalidUserIdException);
    EXPECT_THROW(u1->setId(-5), InvalidUserIdException);
}

// Test del metodo `setName()`
TEST_F(UserTest, SetName) {
    u1->setName("Daniel");
    EXPECT_EQ(u1->getName(), "Daniel");
}

// Test dell'operatore di uguaglianza (==)
TEST_F(UserTest, EqualityOperator) {
    EXPECT_TRUE(*u1 == *u2);  // Dovrebbero essere uguali
    EXPECT_FALSE(*u1 == *u3); // Dovrebbero essere diversi
}

