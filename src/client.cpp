// Just client side

#include <boost/asio.hpp>
#include <iostream>
#include <string>

namespace Network {
    using boost::asio::ip::tcp;

    class client {
    public:
        client(boost::asio::io_context& io_context, const std::string& host, short port)
            : socket_(io_context)
        {
            tcp::resolver resolver(io_context);
            auto endpoints = resolver.resolve(host, std::to_string(port));
            boost::asio::connect(socket_, endpoints);
        }

        void send_message(const std::string& message)
        {
            boost::asio::write(socket_, boost::asio::buffer(message));
        }

        void receive_message()
        {
            char data[1024];
            size_t length = socket_.read_some(boost::asio::buffer(data));
            std::cout << "Received: " << std::string(data, length) << std::endl;
        }

    private:
        tcp::socket socket_;
    };
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: client <host> <port>\n";
            return 1;
        }

        std::string host = argv[1];
        short port = std::atoi(argv[2]);

        boost::asio::io_context io_context;
        Network::client c(io_context, host, port);

        std::string message = "Hello from client!";
        c.send_message(message);
        c.receive_message();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
