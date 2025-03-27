#include <gtest/gtest.h>
#include <memory>
#include "user.h"

class UserTest : public ::testing::Test {
protected:
    std::unique_ptr<user> u1;
    std::unique_ptr<user> u2;
    std::unique_ptr<user> u3;

    void SetUp() override {
        u1 = std::make_unique<user>("Alice", 1);
        u2 = std::make_unique<user>("Alice", 1);
        u3 = std::make_unique<user>("Bob", 2);
    }
};

TEST_F(UserTest, ConstructorValid) {
    EXPECT_EQ(u1->getName(), "Alice");
    EXPECT_EQ(u1->getId(), 1);
}

TEST_F(UserTest, ConstructorInvalidId) {
    EXPECT_THROW(user("Alice", -1), InvalidUserIdException);
    EXPECT_THROW(user("Bob", 0), InvalidUserIdException);
}

TEST_F(UserTest, SetIdValid) {
    u1->setId(10);
    EXPECT_EQ(u1->getId(), 10);
}

TEST_F(UserTest, SetIdInvalid) {
    EXPECT_THROW(u1->setId(0), InvalidUserIdException);
    EXPECT_THROW(u1->setId(-5), InvalidUserIdException);
}

TEST_F(UserTest, SetName) {
    u1->setName("Daniel");
    EXPECT_EQ(u1->getName(), "Daniel");
}

TEST_F(UserTest, EqualityOperator) {
    EXPECT_TRUE(*u1 == *u2);
    EXPECT_FALSE(*u1 == *u3);
}


