#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
#include <future>

#include <boost/asio.hpp>
#include <crow.h>
#include <zlib.h>
#include <nlohmann/json.hpp>

#include <rec_engine.h>
#include <domain_probe.h>
#include <keypass_probe.h>
#include <hasher.h>
#include <id_schema.h>
#include <id_reader.h>
#include <space_c_engine.h>

using namespace std;


nlohmann::json id_reader(std::string id,bool del_id=false){

    id_reader_handler.current_user=schema.current_user;
    id_reader_handler.current_space=schema.current_space;
    id_reader_handler.current_time_id_len=schema.current_time_id_len;

    id_info id_data;

    std::unique_ptr<short> nth_0=std::make_unique<short>();
    *nth_0=std::stoi(id.substr(0,1));

   switch(*nth_0){
    case 0:
    case 2:id_data=id_reader_handler.analyse_02(id);break;
    case 1:
    case 4:id_data=id_reader_handler.analyse_14(id);break;
    case 3:
    case 5:id_data=id_reader_handler.analyse_35(id);break;
   };


   if(del_id==true){
        if(id_data.useCase=="user"){
            schema.del_user(id_data.pos_on_server);
        } else if(id_data.useCase=="space"){
            schema.del_space(id_data.pos_on_server);
        };
   };


nlohmann::json user_info;
nlohmann::json space_info;

if (id_data.user != nullptr) {
    user_info = {
        {"useCase", id_data.user->useCase},
        {"time", id_data.user->time},
        {"position on server", id_data.user->pos_on_server},
    };
} else {
    user_info = nullptr;
};

if (id_data.space_id != nullptr) {
    space_info = {
        {"useCase", id_data.space_id->useCase},
        {"time", id_data.space_id->time},
        {"position on server", id_data.space_id->pos_on_server},
    };
} else {
    space_info = nullptr;
};


nlohmann::json id_info_json{
    {"useCase", id_data.useCase},
    {"time", id_data.time},
    {"Position on Server", id_data.pos_on_server},
    {"user", user_info},
    {"space_id",space_info}
};

    return id_info_json;
};




void d_r_controller(std::string id,std::string topic){

    nlohmann::json json_id_info=id_reader(id);

    std::string useCase=json_id_info["useCase"].get<std::string>();
    std::string time=json_id_info["time"].get<std::string>();
    std::string pos_server=json_id_info["pos_on_server"].get<std::string>();

    

};





string reader(std::vector<unsigned char> x){
    std::unique_ptr<string> out=std::make_unique<string>();

    short i=0;

    while(i<x.size()){
        *out+=x.at(i);
        i++;
    };
    return *out;
};





string compressor(string data){

    std::unique_ptr<string> junk=std::make_unique<string>();

    std::unique_ptr<std::vector<unsigned char>> com_data=std::make_unique<std::vector<unsigned char>>();

    z_stream stream;
    memset(&stream,0,sizeof(stream));
    int* level=new int;
    *level=Z_DEFAULT_COMPRESSION;

    int* status=new int;
    *status=deflateInit(&stream,*level);

    if(*status != Z_OK){
        // "Error Compressing";
        compressor(data);
    };

    com_data->resize(compressBound(data.size()));


    stream.next_in = (Bytef *)data.data();
    stream.avail_in = (uInt)data.size();
    stream.next_out = com_data->data();
    stream.avail_out = com_data->size();


    *status = deflate(&stream, Z_FINISH);
    if (*status != Z_STREAM_END) {
        // "Compression Didnt complete";
         compressor(data);
    };

    deflateEnd(&stream);

    int* compressed_size=new int;
    *compressed_size = com_data->size() - stream.avail_out;
    com_data->push_back(*compressed_size);
    *junk=reader(*com_data);

    return *junk;
    delete compressed_size;
    delete status;
};




string hasher(string key){
    std::unique_ptr<string> message=std::make_unique<string>();

    *message=hasher_ref.hash(key);

    return *message;
};




//recommended topics to query dataBase statistically ...Based on your selected topics
void rec_engine(){
    // specifying number of items to query
    global_t_operands.t_per_page=7;


    // saved data of what user wants
    // global_t_operands.global_t; // vector of topics user wants
    // global_t_operands.weights; // vector of to pick ratings

    // recording what user sees
    global_t_operands.read_t="sport";
    global_t_operands.t_rating=1;

// dont touch....initializers
    global_t_operands.global_checks();
    global_t_operands.query_dataBase();

    for(int i=0;i<global_t_operands.consumer.size();i++){
        cout<<global_t_operands.consumer.at(i)<<endl;
    };
};




bool space_creator(std::string space_id,std::string ownership_id="",std::string space_name="",std::string owner_long_lat[]=nullptr,bool options[]=nullptr){
    std::unique_ptr<bool> is_created=std::make_unique<bool>();
    
        *is_created=creator.create(space_id,ownership_id,space_name,owner_long_lat,options);

    return *is_created;
};


// get awaiting users in a space in a json format for response
nlohmann::json get_awaiting(std::string space_id,std::string owners_id){
    nlohmann::json in_json;

    in_json["user"]=creator.get_awaiting(space_id,owners_id);

    return in_json;
};


nlohmann::json get_piece(std::string space_id){
  nlohmann::json in_json;

    in_json["users"]=creator.get_space_piece(space_id);

  return in_json;  
};


int main(){

// dont touch this blog

    // E_probe_class E_probe;
    // std::unique_ptr<bool> email_probe_handler_result=std::make_unique<bool>(false); // holds the entire email checks results


    // *email_probe_handler_result=E_probe.validator();//mail address goes into the braces


    // E_probe.push_option;/*push option in here. 0 for phone and 1 for gmail push*
    // E_probe.phone_no;/*phone number in here. stringifies*/

    // std::promise<bool> probe_promise;
    // std::future<bool> probe_promise_holder=probe_promise.get_future();


    // std::thread veri_checks_thread([&E_probe,&probe_promise](){

    //     std::unique_ptr<bool> veri_checksResult=std::make_unique<bool>(false);
    //     *veri_checksResult=E_probe.veri_checks();
    //     probe_promise.set_value(*veri_checksResult);

    // });


    // while(E_probe.met_condition!=true){
    //     E_probe.returned_token; // returned token in here
    //     std::this_thread::sleep_for(std::chrono::seconds(3));
    // };

    // veri_checks_thread.join();

    // *email_probe_handler_result=probe_promise_holder.get();


creator_operands space_operations;
    std::string space_id=schema.generate_id("2","0","0");
    std::string user_id=schema.generate_id("0","0","0");
    std::string space_name="telecom_campus base";
    std::string owner_long_lat[2];
    bool options[4]={false,false,false,false};

    // cout<<space_creator(space_id,user_id,space_name,owner_long_lat,options)<<endl;

    // std::promise<bool> get_test;
    // std::future<bool> get_test_holder=get_test.get_future();

    // std::thread testing_thread([&space_operations,&get_test,&space_id,&user_id](){
    //     std::unique_ptr<bool> isOpen=std::make_unique<bool>();
    //     *isOpen=creator.open_space(space_id,user_id,120);
    //     get_test.set_value(*isOpen);
    // });

    // for(int i=0;i<100;i++){
    //     creator.join(space_id,user_id);
    // };

    // testing_thread.join();

    // cout<<"space still open ?...."<<get_test_holder.get()<<endl;


    d_r_controller(schema.generate_id("0","0","0"),"football");

    return 0;
}