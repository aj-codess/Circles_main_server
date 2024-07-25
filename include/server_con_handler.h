#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <future>
#include <memory>
#include <functional>
#include <utility>

#include <boost/asio.hpp>
#include <socket_in_thread_handler.h>

using namespace std;

thread_handler thread_operands;

struct domain_details {
    std::string host_url;
    std::string port;
};


class server_operands {
private:
    boost::asio::io_context& ioc;
    boost::asio::ip::tcp::endpoint circles_endpoint;
    boost::asio::ip::tcp::acceptor l_acceptor;
    boost::asio::ip::tcp::resolver resolver;
    bool isOpen;
    void open_acceptor();
    void accept_client();

public:
    server_operands(boost::asio::io_context& context)
        : ioc(context), l_acceptor(context), resolver(context), isOpen(false) {};


    void open_lis_con(const domain_details& domain_dtl);
    bool get_status() const { return isOpen; };
};




void server_operands::open_lis_con(const domain_details& domain_dtl) {

    try {

        boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(domain_dtl.host_url, domain_dtl.port);
        
        circles_endpoint = *endpoints.begin();

        open_acceptor();

    } catch (const exception& e) {

        cout << "not a registered Host - " << e.what() << endl;

    };

}

void server_operands::open_acceptor() {

    l_acceptor.open(circles_endpoint.protocol());

    l_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));

    l_acceptor.bind(circles_endpoint);

    l_acceptor.listen();

    cout << "Server endpoint is - " << circles_endpoint << endl;

    accept_client();
};





void server_operands::accept_client() {

    auto socket = make_shared<boost::asio::ip::tcp::socket>(ioc);

    l_acceptor.async_accept(*socket, [this, socket](const boost::system::error_code& error) {
        
        if (!error) {

            if (socket->is_open()) {

                isOpen = true;

                cout << "Client accepted with remote endpoint - " << socket->remote_endpoint() << endl;
            
                

            }

            accept_client();

        } else {

            cout << "error accepting - " << error.message() << endl;

            open_acceptor();

        }
    });
}