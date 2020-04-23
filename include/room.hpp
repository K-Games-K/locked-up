#pragma once

#include <string>

class Room
{
private:
    std::string name;

public:
    Room(const std::string& name);

    std::string get_name() const;

    bool operator==(const Room& other);
};
