#include <iostream>
#include <string>
#include <thread>
#include <memory>

#include <boost/asio.hpp>

#include <server_con_handler.h>
#include <id_gen_connector.h>
#include <fstream>
#include <hasher.h>
#include <compressor.h>

using namespace std;

domain_details id_serverDomain;

class network_operands {
public:
    void start_server(domain_details& domain);
    
    network_operands(boost::asio::io_context& context) : server(context),id_pipe(context,{"localhost","8080"}) {

        auto env_writter=[&](){

            std::ofstream path(ENV);

            if (path.is_open()) {

                try{

                    std::string gen=hasher.hash(shrink.compress(GLOBAL));

                    path<<gen<<endl;

                    path.close();

                } catch(const std::exception e){

                    cout<<"unable to write env - "<<e.what()<<endl;

                };

            };
        };

        env_writter();

    };


private:
    server_operands server;
    id_gen_con id_pipe;
    hasher_class hasher;
    shrink_operands shrink;
};



void network_operands::start_server(domain_details& domain) {

    try {

        server.open_lis_con(domain);

    } catch (const exception& e) {

        cout << "Error Starting Server - " << e.what() << endl;
    };

};




int main() {

    domain_details domain_dtl = { "localhost", "8888" };

    boost::asio::io_context context;

    network_operands network(context);

    network.start_server(domain_dtl);

    context.run();

    return 0;
}