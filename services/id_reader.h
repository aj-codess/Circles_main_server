#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <array>
#include <chrono>
#include <functional>

#include <id_schema.h>
#include <nlohmann/json.hpp>


using namespace std;

class id_reader_class{
    private:
    uint64_t current_user_n;
    uint64_t current_space_n;
    short current_time_len;
    id_info analyse_02(std::string id);
    id_info analyse_14(std::string id);
    id_info analyse_35(std::string id);

    public:
    id_reader_class()=default;
    std::string get_timestamp(std::string string_time_junk);
    id_info id_read(std::string id);
    nlohmann::json get_in_json(id_info id_info);
} id_reader_handler;


std::string useCase[6]={"user","ugc","space","space_ugc","chat","space_c"};


std::string id_reader_class::get_timestamp(std::string string_time_junk){

    time_t time_value=std::stoll(string_time_junk);

    decltype(time_value) divider=(time_value/1000);

    char* junk_time=std::ctime(&divider);

    return std::string(junk_time);
};




id_info id_reader_class::analyse_02(std::string id){

    id_info analyse_02_struct;

    std::unique_ptr<short> use=std::make_unique<short>();

    *use=std::stoi(id.substr(0,1));

    analyse_02_struct.useCase=useCase[*use];

    std::unique_ptr<std::string> timeStamp=std::make_unique<std::string>();

    *timeStamp=id.substr(1,this->current_time_len);

    analyse_02_struct.time=this->get_timestamp(*timeStamp);


    std::unique_ptr<std::string> pos=std::make_unique<std::string>();

    *pos=id.substr(this->current_time_len+1);
    
    analyse_02_struct.pos_on_server=*pos;
    
    analyse_02_struct.user=nullptr;
    
    analyse_02_struct.space_id=nullptr;

    return analyse_02_struct;
};



id_info id_reader_class::analyse_14(std::string id){

    id_info analyse_14_struct;


    std::unique_ptr<short> use=std::make_unique<short>();

    *use=std::stoi(id.substr(0,1));


    analyse_14_struct.useCase=useCase[*use];


    std::unique_ptr<std::string> timeStamp=std::make_unique<std::string>();

    *timeStamp=id.substr(1,this->current_time_len);

    analyse_14_struct.time=this->get_timestamp(*timeStamp);



    analyse_14_struct.user=new id_info(this->analyse_02(id.substr(this->current_time_len+1)));

    analyse_14_struct.space_id=nullptr;

    return analyse_14_struct;
};




id_info id_reader_class::analyse_35(std::string id){

    id_info analyse_35_struct;


    std::unique_ptr<short> use=std::make_unique<short>();

    *use=std::stoi(id.substr(0,1));



    analyse_35_struct.useCase=useCase[*use];



    std::unique_ptr<std::string> timeStamp=std::make_unique<std::string>();

    *timeStamp=id.substr(1,this->current_time_len);

    analyse_35_struct.time=this->get_timestamp(*timeStamp);



    analyse_35_struct.user=new id_info(this->analyse_02(id.substr(1+this->current_time_len,1+this->current_time_len+std::to_string(this->current_user_n).length())));

    analyse_35_struct.space_id=new id_info(this->analyse_02(id.substr(2+(this->current_time_len*2)+std::to_string(this->current_user_n).length())));
    
    
    return analyse_35_struct;
};




id_info id_reader_class::id_read(std::string id){
       id_info id_data;

       schema.persistent_init([&](persistent persist){
                this->current_user_n=persist.current_user_n;
                this->current_space_n=persist.current_space_n;
                this->current_time_len=persist.current_time_len;
       });

    std::unique_ptr<short> nth_0=std::make_unique<short>();

    *nth_0=std::stoi(id.substr(0,1));

   switch(*nth_0){

    case 0:
    case 2:id_data=this->analyse_02(id);break;

    case 1:
    case 4:id_data=this->analyse_14(id);break;

    case 3:
    case 5:id_data=this->analyse_35(id);break;

   };

    return id_info_json;
};



nlohmann::json id_reader_class::get_in_json(id_info id_info){
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
}