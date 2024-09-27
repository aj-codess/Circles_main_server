#pragma once

#include <iostream>
#include <string>
#include <memory>

#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/coroutine/all.hpp>
#include <boost/beast.hpp>

using namespace std;

struct domain_struc{
    std::string host;
    std::string port;
};


class id_gen_con{
    private:
    boost::asio::io_context& ioc;
    domain_struc domain_dtl;
    boost::asio::ip::tcp::resolver resolver;
    boost::asio::ip::tcp::resolver::results_type resolved_address;
    void io_pipe(boost::asio::yield_context yield_ioc);

    public:
        id_gen_con()=default;
        id_gen_con(boost::asio::io_context& context,domain_struc domain)
        :ioc(context),domain_dtl(domain),resolver(boost::asio::make_strand(context)) {
            try{

                this->server_init();

            } catch(const exception& e){

                cout<<"Error connecting to id server - "<<e.what()<<endl;

            };
        };

    void server_init();
    void connect_init();
};



void id_gen_con::server_init(){

    this->resolver.async_resolve(domain_dtl.host,domain_dtl.port,[this](boost::beast::error_code ec,boost::asio::ip::tcp::resolver::results_type resolved_address){
        if(ec){

            cout<<"failed resolving - "<<ec.message()<<endl;

        } else{

            this->resolved_address=std::move(resolved_address);

            if(this->resolved_address.empty())
            cout<<"shid didnt work "<<endl;
        };

    });

};



// void id_gen_con::connect_init(){

//     boost::asio::spawn(ioc,[this](boost::asio::yield_context yield_ioc){
//         this->io_pipe(yield_ioc);
//     });

// };


// void id_gen_con::io_pipe(boost::asio::yield_context yield_ioc){

//     struct con{
//         boost::beast::tcp_stream stream_socket;
//         boost::beast::flat_buffer buffer={};
//         boost::beast::http::request<boost::beast::http::string_body> req={};
//         boost::beast::http::response<boost::beast::http::string_body> res={};
//     };

//     auto state=std::make_shared<con>(con{boost::beast::tcp_stream{boost::asio::make_strand(ioc)}});

//     state->stream_socket.expires_after(std::chrono::seconds(30));

//     try{

//     boost::asio::ip::tcp::endpoint endpoint_=*this->resolved_address.begin();

//         state->stream_socket.async_connect(resolved_address,yield_ioc,
//         [state](boost::beast::error_code ec,boost::asio::ip::tcp::resolver::results_type){

//             state->stream_socket.expires_after(std::chrono::seconds(30));

//             if(!ec){

//                 boost::beast::http::async_write(state->stream_socket,state->req,[state](boost::beast::error_code ec,std::size_t size){
//                     boost::ignore_unused(size);

//                     if(ec){
//                         cout<<"Error Writing - "<<ec.message()<<endl;
//                         //retry
//                     } else{

//                         boost::beast::http::async_read(
//                                     state->stream_socket, state->buffer, state->res,
//                                     [state](boost::beast::error_code ec, size_t transfered_size) mutable {
//                                         boost::ignore_unused(transfered_size);

//                                         if (ec) {
//                                             cout<<"error reading from remote Server - "<<ec.message()<<endl;
//                                         };

//                                     });

//                                 if (state->req.need_eof()) {

//                                     boost::beast::error_code shutdown_ec;

//                                     state->stream_socket.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_send,
//                                                                            shutdown_ec);

//                                     if (shutdown_ec) {
//                                         std::cout << "Error shutting down: " << shutdown_ec.message()
//                                                   << std::endl;
//                                     }
//                                 };

//                                 };
//                 });

//             } else{
//                 cout<<"error connecting - "<<ec.message()<<endl;
//             };

//         });

//     } catch(const exception& e){
//         cout<<"Error Connecting - "<<e.what()<<endl;
//     };

// };