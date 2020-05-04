#include <chrono>
#include <iomanip>

#include "logging.hpp"

const char* Log::date_format = "%d-%m-%Y %H:%M:%S";

std::ostream& Log::info(std::ostream& out)
{
    return print_time(out, "INFO");
}

std::ostream& Log::warn(std::ostream& out)
{
    return print_time(out, "WARNING");
}

std::ostream& Log::error(std::ostream& out)
{
    return print_time(out, "ERROR");
}

std::ostream& Log::debug(std::ostream& out)
{
    return print_time(out, "DEBUG");
}

std::ostream& Log::print_time(std::ostream& out, const std::string& level)
{
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    return out << "[" << std::put_time(std::localtime(&now), date_format) << "][" << level << "] ";
}
