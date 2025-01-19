#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <future>
#include <memory>
#include <functional>
#include <utility>

#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/coroutine/all.hpp>
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>

#include <req-res_handler.h>

using namespace std;


struct domain_details {
    std::string host_url;
    std::string port;
};


class server_operands {
private:
    req_res_handler req_res;
    boost::asio::io_context& ioc;
    boost::asio::ip::tcp::endpoint circles_endpoint;
    boost::asio::ip::tcp::acceptor l_acceptor;
    boost::asio::ip::tcp::resolver resolver;
    bool isOpen;
    void open_acceptor();
    void accept_client();
    void socket_handler(std::shared_ptr<boost::asio::ip::tcp::socket> socket,boost::asio::yield_context yield_ioc);

public:
    server_operands(boost::asio::io_context& context)
        : ioc(context), l_acceptor(context), resolver(context), isOpen(false) {

            cout<<"server Loaded "<<endl;

            this->req_res.initial=true;

        };


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

                boost::asio::spawn(l_acceptor.get_executor(),[this,socket](boost::asio::yield_context yield_ioc){
                    this->socket_handler(socket,yield_ioc);
                });

            }

            this->accept_client();

        } else {

            cout << "error accepting - " << error.message() << endl;

            open_acceptor();

        }
    });
};



void server_operands::socket_handler(std::shared_ptr<boost::asio::ip::tcp::socket> socket,boost::asio::yield_context yield_ioc){

    boost::beast::tcp_stream stream_socket(std::move(*socket));

    for(;;){
        bool isDisconnected=false;

        try{
            boost::beast::flat_buffer buffer;

            boost::beast::http::request<boost::beast::http::string_body> req;

            boost::beast::http::response<boost::beast::http::string_body> res;

            boost::beast::http::async_read(stream_socket,buffer,req,yield_ioc);

            req_res.structure(req,res,stream_socket,[&](){

                //switch to websocket in this session when necessary

            });

            boost::beast::http::async_write(stream_socket,res,yield_ioc);
   
            if (res.need_eof()) {

                boost::beast::error_code shutdown_ec;

                stream_socket.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_send, shutdown_ec);

                isDisconnected = true;

                if (shutdown_ec) {

                    cout << "Error shutting down: " << shutdown_ec.message() << endl;

                };

                break;
            };

        } catch(const std::exception& e){
            cout<<"Error with session - "<<e.what()<<endl;
        };

        if(isDisconnected==true){
            break;
        }
    }
};