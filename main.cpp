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


struct domain_details{
    std::string host_url;
    std::string port;
};


class server{
    private:

    public:
    void open_lis_con(boost::asio::io_context &context,domain_details domain_dtl);
};



void server::open_lis_con(boost::asio::io_context &context,domain_details domain_dtl){

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

        boost::asio::ip::tcp::socket socket(context);

        listener_acceptor.accept(socket);

        cout<<"new connection from - "<<socket.remote_endpoint()<<endl;

        

    };

} catch(const exception & e){
    cout<<"not a registered Host - "<<e.what()<<endl;
};

};




int main(){

try{
    domain_details domain_dtl={"localhost","8888"};

    server socket;

    boost::asio::io_context context;

    socket.open_lis_con(context,domain_dtl);
}catch(const exception & e){
    cout<<"there was an error starting server - "<<e.what()<<endl;
};



    // while(true){
    //     boost::asio::ip::tcp::socket socket(context_layer);
        
    //     listener_acceptor.accept(socket);

    //     cout<<"new connection from - "<<socket.remote_endpoint()<<endl;

    //     std::thread(socket_handler,std::ref(socket)).detach();

    // };

    
    return 0;
}
