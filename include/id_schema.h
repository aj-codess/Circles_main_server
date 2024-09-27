#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <memory>
#include <vector>

#include <id_gen_connector.h>

using namespace std;

class id_schema{
    public:
    uint64_t user_space_position(bool user_or_space);
    std::string generate_id(std::string option,std::string user_id="0",std::string space_id="0");
    bool del_user(std::string user_id,std::string user_pos);
    bool del_space(std::string space_id,std::string space_pos);
    bool del_ugc(std::string user_id,std::string ugc_id);

    private:
    id_gen_con id_server;
} schema;

//update current and unoccupied from dataabase.




string id_schema::generate_id(std::string option,std::string user_id_f_ugc,std::string space_id_f_ugc){
    std::string id;

    return id;
};



bool id_schema::del_user(std::string user_id,std::string user_pos){
    bool isDeleted;
    
    return isDeleted;
};


bool id_schema::del_space(std::string space_id,std::string space_pos){
    bool isDeleted;

    return isDeleted;
};