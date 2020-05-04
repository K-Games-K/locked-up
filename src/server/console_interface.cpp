#include <iostream>
#include <sstream>

#include "server/console_interface.hpp"

ConsoleInterfrace::ConsoleInterfrace()
    : worker_thread(&ConsoleInterfrace::console_interface_worker, this) {}

void ConsoleInterfrace::register_command(const std::string& command, Command callback)
{
    commands.insert({command, callback});
}

void ConsoleInterfrace::shutdown()
{
    enabled = false;
    worker_thread.join();
}

void ConsoleInterfrace::console_interface_worker()
{
    while(enabled)
    {
        std::string line;
        std::getline(std::cin >> std::ws, line);

        if(line.empty())
            continue;

        std::stringstream ss(line);
        std::vector<std::string> args;
        while(!ss.eof())
        {
            std::string arg;
            ss >> arg;
            std::transform(
                arg.begin(), arg.end(), arg.begin(), [](char c) { return std::tolower(c); }
            );
            args.push_back(arg);
        }

        if(args[0] == "help")
            print_help();
        else if(commands.find(args[0]) != commands.end())
            commands.at(args[0])(args);
        else
            std::cout << "Wrong command. Type \"help\" for list of available comamnds." << std::endl;
    }
}

void ConsoleInterfrace::print_help()
{
    std::cout << "Available commands:" << std::endl;
    for(auto& command : commands)
        std::cout << " - " << command.first << std::endl;
}
