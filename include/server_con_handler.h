#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <future>
#include <memory>
#include <functional>
#include <utility>

#include <boost/asio.hpp>

using namespace std;


struct domain_details{
    std::string host_url;
    std::string port;
};



class server_operands{
    private:
    void socket_handler(boost::asio::ip::tcp::socket socket);
    void open_acceptor();
    boost::asio::ip::tcp::endpoint circles_endpoint;
    boost::asio::ip::tcp::acceptor l_acceptor;
    boost::asio::io_context& i_o_context;
    boost::asio::io_context::work idle_work;
    boost::asio::ip::tcp::resolver resolver;
    void accept_client();

    public:
    bool isOpen=false;
    void open_lis_con(domain_details domain_dtl);

        // Default constructor
    server_operands()
        : l_acceptor(*(new boost::asio::io_context())), idle_work(*(new boost::asio::io_context())), resolver(*(new boost::asio::io_context())), i_o_context(*(new boost::asio::io_context())) {};

    server_operands(boost::asio::io_context& context)
        : l_acceptor(context), idle_work(context), resolver(context), i_o_context(context) {};

} server;




void server_operands::open_acceptor() {

    this->l_acceptor.open(this->circles_endpoint.protocol());
    
    this->l_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    
    this->l_acceptor.bind(this->circles_endpoint);
    
    this->l_acceptor.listen();
    
    std::cout << "Server endpoint is - " << this->circles_endpoint << std::endl;
    
    this->accept_client();
};



void server_operands::accept_client() {
    auto new_socket = std::make_shared<boost::asio::ip::tcp::socket>(i_o_context);

    this->l_acceptor.async_accept(*new_socket, [this, new_socket](boost::system::error_code ec) {
        if (!ec) {
            this->isOpen = true;
            try {
                if (new_socket->is_open()) {
                    std::cout << "Socket is open." << std::endl;
                    std::cout << "New request from " << new_socket->remote_endpoint() << std::endl;
                } else {
                    std::cerr << "Socket is not open." << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "Error getting remote endpoint: " << e.what() << std::endl;
            }

            // Handle the new connection (for example, start a thread to handle it)
            // std::thread(&server_operands::socket_handler, this, new_socket).detach();

            // Prepare to accept the next connection
            this->accept_client();
        } else {
            this->isOpen = false;
            std::cerr << "Accept error: " << ec.message() << std::endl;

            // Retry accepting connections
            this->open_acceptor();
        }
    });
}



void server_operands::open_lis_con(domain_details domain_dtl){

try{
    
    boost::asio::ip::tcp::resolver::results_type endpoints = this->resolver.resolve(domain_dtl.host_url,domain_dtl.port);

    this->circles_endpoint = *endpoints.begin();

    this->open_acceptor();

} catch(const exception& e){
    cout<<"not a registered Host - "<<e.what()<<endl;
};

};




void server_operands::socket_handler(boost::asio::ip::tcp::socket socket){

    boost::system::error_code ec;

    cout<<"the fuck is all this"<<endl;
    
};
