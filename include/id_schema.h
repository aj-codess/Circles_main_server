#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <memory>
#include <vector>

using namespace std;

class id_schema{
    public:
    time_t time_id();
    uint64_t user_space_position(bool user_or_space);
    std::string generate_id(std::string option,std::string user_id_f_ugc="0",std::string space_id_f_ugc="0");
    void del_user(std::string user_id);
    void del_space(std::string space_id);

    uint64_t current_user;
    std::vector<uint64_t> unoccupied_user;
    uint64_t current_space;
    std::vector<uint64_t> unoccupied_space;
    short current_time_id_len;
} schema;

//update current and unoccupied from dataabase.


//get the current time as id
time_t id_schema::time_id(){
    std::time_t server_time=std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    this->current_time_id_len=std::to_string(server_time).length();
    return server_time;
};



uint64_t id_schema::user_space_position(bool user_or_space){
    std::unique_ptr<uint64_t> position=std::make_unique<uint64_t>();

if(user_or_space==false){
    if(!this->unoccupied_user.empty()){
        *position=this->unoccupied_user.front(); 
        this->unoccupied_user.erase(this->unoccupied_user.begin());
    } else{
        ++this->current_user;
        *position=this->current_user;
    };
} else{
        if(!this->unoccupied_space.empty()){
        *position=this->unoccupied_space.front();
        this->unoccupied_space.erase(this->unoccupied_space.begin());
    } else{
        ++this->current_space;
        *position=this->current_space;
    };
};
    return *position;
};




string id_schema::generate_id(std::string option,std::string user_id_f_ugc,std::string space_id_f_ugc){
    std::unique_ptr<std::string> gen=std::make_unique<std::string>();

    if(option=="0"){
        *gen=option+std::to_string(this->time_id())+std::to_string(this->user_space_position(false));//user
    } else if(option=="1"){
        *gen=option+std::to_string(this->time_id())+user_id_f_ugc;//ugc
    } else if(option=="2"){
        *gen=option+std::to_string(this->time_id())+std::to_string(this->user_space_position(true));//space
    } else if(option=="3") {
        *gen=option+std::to_string(this->time_id())+user_id_f_ugc+space_id_f_ugc;//space ugc
    } else if(option=="4"){
        *gen=option+std::to_string(this->time_id())+user_id_f_ugc; //chat
    } else{
        *gen=option+std::to_string(this->time_id())+user_id_f_ugc+space_id_f_ugc; //space c;
    };

return *gen;
};



void id_schema::del_user(std::string user_id){
    this->unoccupied_user.push_back(std::stoull(user_id));
};


void id_schema::del_space(std::string space_id){
    this->unoccupied_space.push_back(std::stoull(space_id));
};