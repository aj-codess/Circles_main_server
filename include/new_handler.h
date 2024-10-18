#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <map>

#include <jsonScript.h>

using namespace std;

    struct user_recog{
        std::string gmail="";
        std::string password="";
        std::string phone="";
        std::string bio_data="";

        user_recog()=default;
    };

struct log_data{
    std::string timer_hold;
    user_recog initials;
    std::string token;
};

class handler{
    public:
    bool exists(std::string id_2_find);
    std::string gen_new();
    void setter(std::string id,login_init log_data);
    std::map<std::string,log_data> logger;
    void set_time_token(std::string id,std::string token_time,std::string token);

    private:

};


std::string handler::gen_new(){

    auto current_time=std::chrono::system_clock::now();

    auto get_duration=current_time.time_since_epoch();

    auto get_in_milli=std::chrono::duration_cast<std::chrono::milliseconds>(get_duration).count();

    return (std::to_string(get_in_milli));
};



bool handler::exists(std::string id_2_find){

    if(this->logger.find(id_2_find) != this->logger.end()){

        return true;

    } else{

        return false;
        
    };
};


void handler::setter(std::string id,login_init log_){

        this->logger[id]=log_data();

        this->logger[id].timer_hold="";

        this->logger[id].initials.gmail=log_.gmail;

        this->logger[id].initials.password=log_.password;

        this->logger[id].initials.bio_data=log_.bio_data;

        this->logger[id].initials.phone=log_.phone;

        this->logger[id].token="";
};


void handler::set_time_token(std::string id,string token_time,std::string token){

    if(this->exists(id)==true){

        this->logger[id].timer_hold=token_time;

        this->logger[id].token=token;

    };

};