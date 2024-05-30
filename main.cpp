#include <iostream>
#include <string>
#include <memory>

#include <server_con_handler.h>

using namespace std;

class network_operands{
    private:

    public:
    bool start_server(boost::asio::io_context& context,domain_details& domain);
} network;



bool network_operands::start_server(boost::asio::io_context& context,domain_details& domain){
    std::unique_ptr<bool> isStarted=std::make_unique<bool>(false);

    try{

        server.open_lis_con(context,domain);

        *isStarted=server.isOpen;

    } catch(const exception& e){
        cout<<"Error Starting Server - "<<e.what()<<endl;
        *isStarted=false;
    }

    return *isStarted;
}



int main(){

    domain_details domain_dtl={"localhost","8888"};

    boost::asio::io_context context;

    network.start_server(context,domain_dtl);

    
    return 0;
}
