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
    bool stop_ioc=false;
    void socket_handler(boost::asio::ip::tcp::socket socket);
    void open_acceptor();
    boost::asio::io_context& ioc;
    boost::asio::ip::tcp::endpoint circles_endpoint;
    boost::asio::ip::tcp::acceptor l_acceptor;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> idle_work;
    boost::asio::ip::tcp::resolver resolver;
    void accept_client();

    public:
    bool isOpen=false;
    void open_lis_con(domain_details domain_dtl);

        // Default constructor
    server_operands()
        : ioc(*(new boost::asio::io_context())),idle_work(boost::asio::make_work_guard(ioc)), l_acceptor(*(new boost::asio::io_context())), resolver(*(new boost::asio::io_context())) {};

        //constructor
     server_operands(boost::asio::io_context& context)
        : ioc(context), l_acceptor(context), idle_work(boost::asio::make_work_guard(context)), resolver(context) {};

} server;



void server_operands::open_acceptor() {

    this->l_acceptor.open(this->circles_endpoint.protocol());
    
    this->l_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    
    this->l_acceptor.bind(this->circles_endpoint);
    
    this->l_acceptor.listen();
    
    std::cout << "Server endpoint is - " << this->circles_endpoint << std::endl;

    this->accept_client();

        std::thread ioc_thread([this]{
            ioc.run();
        });


        if(ioc_thread.joinable()){
            ioc_thread.join();
        };
    
    //this->open_acceptor();

};




void server_operands::accept_client() {

    auto socket=make_shared<boost::asio::ip::tcp::socket>(ioc);

    this->l_acceptor.async_accept(*socket, [this,socket](const boost::system::error_code& error){

        if(error){
            cout<<"error accepting - "<<error.message()<<endl;

            this->stop_ioc=true;

            this->open_acceptor();
        };


        try{

            if(socket->is_open()){

                this->isOpen=true;

                cout<<"Client accepted with remote enpoint - "<<socket->remote_endpoint()<<endl;

            };

            this->accept_client();

        } catch(const exception& e){

            cout<<"hold in the accepting process - "<<e.what()<<endl;

            this->accept_client();

        };

    });

};




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
