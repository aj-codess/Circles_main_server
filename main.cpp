#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
#include <future>

#include <boost/asio.hpp>
#include <crow.h>
#include <pqxx/pqxx>
#include <nlohmann/json.hpp>

#include <openssl/opensslconf.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <rec_engine.h>
#include <circles_rec_engine.h>
#include <domain_probe.h>
#include <keypass_probe.h>
#include <hasher.h>
#include <id_schema.h>
#include <space_c_engine.h>
#include <compressor.h>
#include <id_reader.h>

using namespace std;


void socket_handler(boost::asio::ip::tcp::socket& socket){
    std::vector<char> mem_buffer(2*1024);

    socket.async_read_some(boost::asio::buffer(mem_buffer.data(),mem_buffer.size()),[&](boost::system::error_code error,std::size_t length){
        if(!error){
            for(int i=0;i<length;i++){
                cout<<mem_buffer[i];
            };
        } else{
            cout<<error.message()<<endl;
        };
        socket_handler(socket);
    });

};


int main(){

    boost::system::error_code grab_error;

    boost::asio::io_context context_layer;

    boost::asio::io_context::work idleWork(context_layer);

    boost::asio::ip::tcp::endpoint circles_endpoint(boost::asio::ip::tcp::v4(),8080);

    boost::asio::ip::tcp::acceptor listener_acceptor(context_layer,circles_endpoint);

    cout<<circles_endpoint<<endl;

    while(true){
        boost::asio::ip::tcp::socket socket(context_layer);
        
        listener_acceptor.accept(socket);

        cout<<"new connection from - "<<socket.remote_endpoint()<<endl;

        std::thread(socket_handler,std::ref(socket)).detach();

    };

    
    return 0;
}
