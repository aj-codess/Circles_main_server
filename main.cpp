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




// this is a different blog....................

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


    return 0;
}