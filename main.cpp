#include <iostream>
#include <string>
#include <thread>
#include <memory>

#include <server_con_handler.h>

using namespace std;

class network_operands{
    public:
    bool close_server();
    bool server_status();
    void start_server(boost::asio::io_context& context,domain_details& domain);

    private:

} network;



void network_operands::start_server(boost::asio::io_context& context,domain_details& domain){
    try{

        server.open_lis_con(context,domain);

        context.run();

    } catch(const exception& e){

        cout<<"Error Starting Server - "<<e.what()<<endl;

    };
};


bool network_operands::server_status(){
    std::unique_ptr<bool> isRunning=std::make_unique<bool>(false);
    
    *isRunning=server.isOpen;

    return *isRunning;
};



int main(){

    domain_details domain_dtl={"localhost","8888"};

    boost::asio::io_context context;

    network.start_server(context,domain_dtl);

    return 0;
}
