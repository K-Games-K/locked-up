#pragma once

#include <iostream>

class Log
{
public:
    static std::ostream& info(std::ostream& out = std::cout);

    static std::ostream& warn(std::ostream& out = std::cerr);

    static std::ostream& error(std::ostream& out = std::cerr);

    static std::ostream& debug(std::ostream& out = std::cout);

private:
    static const char* date_format;

    static std::ostream& print_time(std::ostream& out, const std::string& level);
};
