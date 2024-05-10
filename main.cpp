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

    in_json["users awaiting"]=creator.get_awaiting(space_id,owners_id);

    return in_json;
};




nlohmann::json get_piece(std::string space_id){
  nlohmann::json in_json;

    in_json["users"]=creator.get_space_piece(space_id);

  return in_json;  
};





nlohmann::json get_admins(std::string space_id){
    nlohmann::json in_json;

    in_json["sub_admins"]=creator.get_space_sub_admins(space_id);

    return in_json
};




int main(){



    return 0;
}
