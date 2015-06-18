//
// handler_allocator.h
//

#pragma once

#include <cstdlib>
#include <array>

class session : public std::enable_shared_from_this<session> {
public:
    session(tcp::socket socket)
        : socket_(std::move(socket))
    {
    }

    void start() {
        this->do_read();
    }

private:
    void do_read() {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(data_),
                make_custom_alloc_handler(allocator_,
                    [this, self](boost::system::error_code ec, std::size_t length) {
                        if (!ec) {
                            do_write(length);
                        }
                    }));
    }

    void do_write(std::size_t length) {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
                make_custom_alloc_handler(allocator_,
                    [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                        if (!ec) {
                            do_read();
                        }
                    }));
    }

    // The socket used to communicate with the client.
    tcp::socket socket_;

    // Buffer used to store data received from the client.
    std::array<char, 1024> data_;

    // The allocator to use for handler-based custom memory allocation.
    HandlerAllocator allocator_;
};


class server
{
public:
    server(boost::asio::io_service& io_service, short port)
        : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
            socket_(io_service)
    {
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept(socket_,
                [this](boost::system::error_code ec) {
                    if (!ec) {
                        std::make_shared<session>(std::move(socket_))->start();
                    }

                    do_accept();
                });
    }

    tcp::acceptor acceptor_;
    tcp::socket socket_;
};

} // namespace streamx
