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
    void open_acceptor(boost::asio::io_context &context,boost::asio::ip::tcp::endpoints &circles_endpoint);
    void accept_conn();

    public:
    bool isOpen=false;
    void open_lis_con(boost::asio::io_context &context,domain_details domain_dtl);
} server;




void server_operands::open_acceptor(boost::asio::io_context &context, boost::asio::ip::tcp::endpoints &circles_endpoint){

    boost::asio::ip::tcp::acceptor l_acceptor(context);

    l_acceptor.open(circles_endpoint.protocol());

    l_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));

    l_acceptor.bind(circles_endpoint);

    l_acceptor.listen();

    cout<<"server Endpoint is - "<<circles_endpoint<<endl;

    this->accept_conn();
};




void server_operands::accept_conn(){
    try{
            listener_acceptor.async_accept(socket_,[this](boost::system::error_code ec){
            if(!ec){
                this->isOpen=true;

                cout<<"new socket connection from  - "<<socket_.remote_endpoint()<<endl;

                auto conn_socket=std::make_shared<boost::asio::ip::tcp::socket>(std::move(socket_))

            } else{

                this->isOpen=false;

                this->accept_conn();

            }
    });
    } catch(const exception& e){
        if(e){
            cout<<"error accepting client - "<<e.what()<<endl;

            // call server refresh if necessary
        }
    };
}




void server_operands::open_lis_con(boost::asio::io_context &context,domain_details domain_dtl){

try{

    boost::system::error_code grab_error;

    boost::asio::io_context::work idle_work(context);

    boost::asio::ip::tcp::resolver resolver(context);
    
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(domain_dtl.host_url,domain_dtl.port);

    boost::asio::ip::tcp::endpoint circles_endpoint = *endpoints.begin();

    this->open_acceptor(context,circles_endpoint);

        // std::thread(&server_operands::socket_handler, this, std::ref(socket)).detach();

} catch(const exception& e){
    cout<<"not a registered Host - "<<e.what()<<endl;
};

};



void server_operands::socket_handler(boost::asio::ip::tcp::socket socket){

    boost::system::error_code ec;

    cout<<"the fuck "<<endl;
    
};
