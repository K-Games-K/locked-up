#include <iostream>
#include <sstream>

#include "utils.hpp"
#include "logging.hpp"
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
            args.push_back(Utils::to_lower_case(arg));
        }

        if(args[0] == "help")
            print_help();
        else if(commands.find(args[0]) != commands.end())
            commands.at(args[0])(args);
        else
            Log::warn() << "Wrong command. Type \"help\" for list of available comamnds." << std::endl;
    }
}

void ConsoleInterfrace::print_help()
{
    Log::info() << "Available commands:" << std::endl;
    for(auto& command : commands)
        Log::info() << " - " << command.first << std::endl;
}
