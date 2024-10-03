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


struct payload_opt{
    bool persistent;
    bool del;
    std::string option="";
    std::string user_id="";
    std::string space_id="";
    std::string ugc_id="";

    payload_opt()=default;
};

class id_gen_con{
    private:
    boost::asio::io_context& ioc;
    domain_struc domain_dtl;
    boost::asio::ip::tcp::resolver resolver;
    boost::asio::ip::tcp::resolver::results_type resolved_address;
    void server_init();

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

    void io_pipe(payload_opt payload,std::function<void(boost::beast::http::response<boost::beast::http::string_body>)> callback);
};



void id_gen_con::server_init(){

    this->resolver.async_resolve(domain_dtl.host,domain_dtl.port,[this](boost::beast::error_code ec,boost::asio::ip::tcp::resolver::results_type resolved_address){
        if(ec){

            cout<<"failed resolving - "<<ec.message()<<endl;

        } else{

            this->resolved_address=std::move(resolved_address);

            if(!this->resolved_address.empty())
            cout<<" Address Resolved "<<endl;

        };

    });

};



void id_gen_con::io_pipe(payload_opt payload,std::function<void(boost::beast::http::response<boost::beast::http::string_body>)> callback){

    struct con{
        boost::beast::tcp_stream stream_socket;
        boost::beast::flat_buffer buffer={};
        boost::beast::http::request<boost::beast::http::string_body> req={};
        boost::beast::http::response<boost::beast::http::string_body> res={};
    };

    auto state=std::make_shared<con>(con{boost::beast::tcp_stream{boost::asio::make_strand(ioc)}});

    state->stream_socket.expires_after(std::chrono::seconds(30));


// struct payload_opt{
//     bool persistent;
//     bool del;
//     std::string option="";
//     std::string user_id="";
//     std::string space_id="";
//     std::string ugc_id="";

//     payload_opt()=default;
// };
    auto req_writter=[&](payload_opt& payload){

        state->req.method(boost::beast::http::verb::get);

    };

    req_writter(payload);

    try{

        state->stream_socket.async_connect(this->resolved_address,
        [state](boost::system::error_code ec,boost::asio::ip::tcp::endpoint ep){

            state->stream_socket.expires_after(std::chrono::seconds(30));

            if(!ec){

                boost::beast::http::async_write(state->stream_socket,state->req,[state](boost::beast::error_code ec,std::size_t size){
                    boost::ignore_unused(size);

                    if(ec){
                        cout<<"Error Writing - "<<ec.message()<<endl;
                        //retry
                    } else{

                        boost::beast::http::async_read(
                                    state->stream_socket, state->buffer, state->res,
                                    [state](boost::beast::error_code ec, size_t transfered_size){
                                        boost::ignore_unused(transfered_size);

                                        if (ec) {
                                            cout<<"error reading from remote Server - "<<ec.message()<<endl;
                                        };

                                            if (state->res.need_eof()) {

                                                boost::beast::error_code shutdown_ec;

                                                state->stream_socket.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_send,
                                                                                    shutdown_ec);

                                                if (shutdown_ec) {
                                                    std::cout << "Error shutting down: " << shutdown_ec.message()
                                                            << std::endl;
                                                }
                                            };
                                    });

                                };
                });

            } else{
                cout<<"Error Connecting to Remote Server - "<<ec.message()<<endl;
            };

        });

    } catch(const exception& e){
        cout<<"Error with session - "<<e.what()<<endl;
    };

};