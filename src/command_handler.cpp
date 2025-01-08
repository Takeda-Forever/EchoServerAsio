#include "../include/command_handler.hpp"

namespace Command
{
    command_handler::command_handler()
    {
        register_command("/print_log", std::make_shared<print_log_command>());
        register_command("/help", std::make_shared<help_command>());
    }
    bool command_handler::execute(const std::string& input)
    {
        std::stringstream ss(input);
        std::string command_name;
        ss >> command_name;

        auto it = commands.find(command_name);
        if (it != commands.end()) 
        {
            std::vector<std::string> args;
            std::string arg;
            while (ss >> arg) {
                args.push_back(arg);
            }
            it->second->complete(args);
        }
        else 
        {
            return false;
        }
        return true;
    }
}