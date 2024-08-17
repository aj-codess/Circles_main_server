#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <memory>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <server_con_handler.h>

using namespace std;

class socket_handler : public std::enable_shared_from_this<socket_handler> {
private:
    boost::asio::ip::tcp::socket socket_addr;

public:
    socket_handler(boost::asio::ip::tcp::socket socket)
        : socket_addr(std::move(socket)) {}

    void io_handler(boost::asio::yield_context yield);
};


void socket_handler::io_handler(boost::asio::yield_context yield) {
    try {

        cout << "This is the socket address - " << socket_addr.remote_endpoint() <<endl;

    } catch (const std::exception& e) {

        cout<< "Exception: " << e.what() <<endl;

    }
};
