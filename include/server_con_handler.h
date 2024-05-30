#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <future>
#include <memory>

#include <boost/asio.hpp>

using namespace std;


struct domain_details{
    std::string host_url;
    std::string port;
};


class server_operands{
    private:


    public:
    bool isOpen;
    void open_lis_con(boost::asio::io_context &context,domain_details domain_dtl);
} server;



void server_operands::open_lis_con(boost::asio::io_context &context,domain_details domain_dtl){

try{

    boost::system::error_code grab_error;

    boost::asio::io_context::work idle_work(context);

    boost::asio::ip::tcp::resolver resolver(context);
    
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(domain_dtl.host_url,domain_dtl.port);

    boost::asio::ip::tcp::endpoint circles_endpoint = *endpoints.begin();

    boost::asio::ip::tcp::acceptor listener_acceptor(context);
    
    listener_acceptor.open(circles_endpoint.protocol());

    listener_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));

    listener_acceptor.bind(circles_endpoint);

    listener_acceptor.listen();

    cout<<"Servers endpoint - "<<circles_endpoint<<endl;

    while(true){
        this->isOpen=true;

        boost::asio::ip::tcp::socket socket(context);

        listener_acceptor.accept(socket);

        cout<<"new connection from - "<<socket.remote_endpoint()<<endl;

        // std::thread(socket_handler,std::ref(socket)).detach();

    };

} catch(const exception & e){
    cout<<"not a registered Host - "<<e.what()<<endl;
};

};