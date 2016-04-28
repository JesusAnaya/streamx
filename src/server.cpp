#include "server.h"
#include <ctime>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace streamx {

std::string Server::make_daytime_string() {
    std::time_t now = time(0);
    return std::ctime(&now);
}

void Server::run() {
    boost::asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 9000));

    while(true) {
        tcp::socket socket(io_service);
        acceptor.accept(socket);

        std::string message = std::string("Anaya says: ") + this->make_daytime_string();

        boost::system::error_code ignored_error;
        boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
    }
}

}; // namespace streamx
