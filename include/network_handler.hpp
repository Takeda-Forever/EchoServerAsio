#pragma once
#include "pch.h"
#include "command_handler.hpp"
#include "log_handler.hpp"

namespace Network {
    using boost::asio::ip::tcp;

    class session
        : public std::enable_shared_from_this<session>
    {
    public:
        session(tcp::socket socket)
            : socket_(std::move(socket))
        {
        }

        void start()
        {
            do_read();
        }

    private:
        void do_read()
        {
            auto self(shared_from_this());
            socket_.async_read_some(boost::asio::buffer(data_, max_length),
                [this, self](boost::system::error_code ec, std::size_t length)
                {
                    if (!ec)
                    {
                        do_write(length);
                    }
                });
        }

        void do_write(std::size_t length)
        {
            auto self(shared_from_this());
            boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
                [this, self](boost::system::error_code ec, std::size_t /*length*/)
                {
                    if (!ec)
                    {
                        do_read();
                    }
                });
        }

        tcp::socket socket_;
        enum { max_length = 1024 };
        char data_[max_length];
    };

    class server
    {
    public:
        server(boost::asio::io_context& io_context, short port)
            : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), ioc(io_context)
        {
            do_accept();
            admin_thread_ = std::thread(&server::admin_control, this);
        }

        virtual ~server() {
            if (admin_thread_.joinable()) {
                admin_thread_.join();
            }
        }
    private:

        void do_stop()
        {
            std::cout << "Stopping server..." << std::endl;
            ioc.stop();
            return;
        }
        
        void do_log(boost::asio::ip::tcp::endpoint& endpoint)
        {
            auto address = endpoint.address().to_string();
            auto port   = std::to_string(endpoint.port());
            auto end_info = "Client " + std::to_string(counter) + " {\n"
                + " - Address: " + address + ";\n"
                + " - Port: " + port + ";\n"
                + "};\n";

            Log::Logger::log
            (
                end_info
            );
        }

        void do_accept()
        {
            acceptor_.async_accept(
                [this](boost::system::error_code ec, tcp::socket socket)
                {
                    if (!ec)
                    {
                        counter++;
                        auto endpoint = socket.remote_endpoint();
                        do_log(endpoint);

                        std::make_shared<session>(std::move(socket))->start();
                    }
                    do_accept();
                });
        }

        void admin_control()
        {
            std::string input;
            while (input != "/exit") {
                std::cout << "> ";
                std::getline(std::cin, input);
                if (!cmd_handler.execute(input) && input != "/exit")
                {
                    std::cout << "Unknown command: " << input << ". Type /help for available commands.\n";
                }
            }
            do_stop();
            std::cout << "Server stopped." << std::endl;
        }

        Command::command_handler cmd_handler;
        tcp::acceptor acceptor_;
        std::thread admin_thread_;
        boost::asio::io_context& ioc;
        unsigned counter = 0;
    };
}