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
    void open_acceptor(boost::asio::io_context &context);
    boost::asio::ip::tcp::endpoint circles_endpoint;

    public:
    bool isOpen=false;
    void open_lis_con(boost::asio::io_context &context,domain_details domain_dtl);
} server;




void server_operands::open_acceptor(boost::asio::io_context &context) {

    boost::asio::ip::tcp::acceptor l_acceptor(context);

    boost::asio::ip::tcp::socket socket(context);

    l_acceptor.open(this->circles_endpoint.protocol());

    l_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));

    l_acceptor.bind(this->circles_endpoint);

    l_acceptor.listen();

    std::cout << "Server endpoint is - " << this->circles_endpoint << std::endl;

    l_acceptor.async_accept(socket, [this, &l_acceptor, &socket, &context](boost::system::error_code ec) {
        if (!ec) {
            this->isOpen = true;
            std::cout << "New socket connection from - " << socket.remote_endpoint() << std::endl;

            auto conn_socket = std::make_shared<boost::asio::ip::tcp::socket>(std::move(socket));

            // Start handling the client connection
            std::thread(&server_operands::socket_handler, this, conn_socket).detach();.

            // Accept the next connection
            this->open_acceptor(context);

        } else {
            this->isOpen = false;
            std::cerr << "Accept error: " << ec.message() << std::endl;

            // Retry accepting connections
            this->open_acceptor(context);
        }
    });

    context.run();

}




void server_operands::open_lis_con(boost::asio::io_context &context,domain_details domain_dtl){

try{

    boost::system::error_code grab_error;

    boost::asio::io_context::work idle_work(context);

    boost::asio::ip::tcp::resolver resolver(context);
    
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(domain_dtl.host_url,domain_dtl.port);

    this->circles_endpoint = *endpoints.begin();

    this->open_acceptor(context);

        // std::thread(&server_operands::socket_handler, this, std::ref(socket)).detach();

} catch(const exception& e){
    cout<<"not a registered Host - "<<e.what()<<endl;
};

};




void server_operands::socket_handler(boost::asio::ip::tcp::socket socket){

    boost::system::error_code ec;

    cout<<"the fuck "<<endl;
    
};
