#pragma once
#include "pch.h"
#include "log_handler.hpp"

namespace Command {
    class command_base {
    public:
        command_base() = default;
        ~command_base() = default;

        command_base(const command_base&) = delete;
        command_base(command_base&&) = delete;
        command_base& operator=(const command_base&) = delete;
        command_base& operator=(command_base&&) = delete;

        virtual void complete(const std::vector<std::string>& args) = 0;
    };
    class print_log_command : public command_base {
    public:
        void complete(const std::vector<std::string>& args) override {
            if (args.empty()) {
                std::cout << "Error: No message provided for /print_log\n";
                return;
            }

            std::cout << "Log: " << args[0] << "\n";
        }
    };
    class help_command : public command_base {
    public:
        void complete(const std::vector<std::string>& args) override {
            std::cout << "Available commands:\n";
            std::cout << "/print_log <message> - Print a log message\n";
            std::cout << "/help - Show this help\n";
            std::cout << "/exit - Shut down the program\n";
        }
    };
    class command_handler {
    public:
        command_handler();
        ~command_handler() 
        {
            unregister_all_commands();
        };

        command_handler(const command_handler&) = delete;
        command_handler(command_handler&&) = delete;
        command_handler& operator=(const command_handler&) = delete;
        command_handler& operator=(command_handler&&) = delete;

        void register_command(const std::string& name, std::shared_ptr<command_base> command) {
            commands[name] = command;
        }

        bool execute(const std::string& input);
    private:
        void unregister_all_commands()
        {
            commands.clear();
        }
        std::map<std::string, std::shared_ptr<command_base>> commands;

    };
}