#include <iostream>
#include <string>
#include <thread>
#include <memory>

#include <boost/asio.hpp>

#include <server_con_handler.h>

using namespace std;

class network_operands {
public:
    void start_server(domain_details& domain);
    
    network_operands(boost::asio::io_context& context) : server(context) {};


private:
    server_operands server;

};

void network_operands::start_server(domain_details& domain) {

    try {

        server.open_lis_con(domain);

    } catch (const exception& e) {

        cout << "Error Starting Server - " << e.what() << endl;
    }

};




int main() {

    domain_details domain_dtl = { "localhost", "8888" };

    boost::asio::io_context context;

    network_operands network(context);

    network.start_server(domain_dtl);

    context.run();

    return 0;
}