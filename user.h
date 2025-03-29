//
// Created by Bianca Cocci on 02/03/25.
//

#ifndef LABORATORIO_USER_H
#define LABORATORIO_USER_H
#include <string>

class InvalidUserIdException : public std::runtime_error {
public:
    explicit InvalidUserIdException(const std::string& message)
            : std::runtime_error(message) {}
};

class user {
public:
    user(const std::string&  name, int id);

    const std::string &getName() const;

    int getId() const;

    void setId(int newid);

    void setName(const std::string &name);

    bool operator==(const user& other) const;

private:
    std::string name;
    int id;
};

#endif //LABORATORIO_USER_H
