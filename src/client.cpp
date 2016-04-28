#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char *argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: client <host>" << std::endl;
            return 1;
        }

        boost::asio::io_service io_service;
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(argv[1], "daytime");
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        tcp::socket socket(io_service);

        // Start connection
        boost::asio::connect(socket, endpoint_iterator);

        boost::array<char, 128> buff;
        boost::system::error_code error;
        size_t len = 0;

        while(true) {
            len = socket.read_some(boost::asio::buffer(buff), error);

            if(error == boost::asio::error::eof) {
                break; // Connection closed cleanly by peer.
            } else if(error) {
                throw boost::system::system_error(error);
            }

            std::cout.write(buff.data(), len);
        }
    } catch(std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
