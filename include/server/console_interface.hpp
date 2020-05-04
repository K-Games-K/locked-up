#pragma once

#include <functional>
#include <thread>
#include <string>
#include <map>

class ConsoleInterfrace
{
public:
    using Command = std::function<void(const std::vector<std::string>&)>;

    ConsoleInterfrace();

    void register_command(const std::string& command, Command callback);

    void shutdown();

private:
    std::thread worker_thread;

    std::map<std::string, Command> commands;

    volatile bool enabled = true;

    void console_interface_worker();

    void print_help();
};
