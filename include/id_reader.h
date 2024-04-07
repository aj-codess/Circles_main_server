#ifndef READER
#define READER

#include <iostream>
#include <string>
#include <memory>
#include <array>

using namespace std;


    struct id_info{
        std::string useCase;
        std::string time;
        std::string pos_on_server;
        id_info* user;
        id_info* space_id;
    };
    

class id_reader_class{
    public:
    uint64_t current_user;
    uint64_t current_space;
    short current_time_id_len;
    std::string get_timestamp(std::string string_time_junk);
    id_info analyse_02(std::string id);
    id_info analyse_14(std::string id);
    id_info analyse_35(std::string id);
} id_reader_handler;



std::string useCase[6]={"user","ugc","space","space_ugc","chat","space_c"};


std::string id_reader_class::get_timestamp(std::string string_time_junk){
    time_t time_value=std::stoll(string_time_junk);
    char* junk_time=std::ctime(&time_value);
    return std::string(junk_time);
};




id_info id_reader_class::analyse_02(std::string id){

    id_info analyse_02_struct;

    std::unique_ptr<short> use=std::make_unique<short>();
    *use=std::stoi(id.substr(0,1));

    analyse_02_struct.useCase=useCase[*use];

    std::unique_ptr<std::string> timeStamp=std::make_unique<std::string>();
    *timeStamp=id.substr(1,this->current_time_id_len);
    analyse_02_struct.time=this->get_timestamp(*timeStamp);

    std::unique_ptr<std::string> pos=std::make_unique<std::string>();
    *pos=id.substr(this->current_time_id_len+1);
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
    *timeStamp=id.substr(1,this->current_time_id_len);
    analyse_14_struct.time=this->get_timestamp(*timeStamp);

    analyse_14_struct.user=new id_info(this->analyse_02(id.substr(this->current_time_id_len+1)));

    analyse_14_struct.space_id=nullptr;

    return analyse_14_struct;
};




id_info id_reader_class::analyse_35(std::string id){
    id_info analyse_35_struct;

    std::unique_ptr<short> use=std::make_unique<short>();
    *use=std::stoi(id.substr(0,1));

    analyse_35_struct.useCase=useCase[*use];

    std::unique_ptr<std::string> timeStamp=std::make_unique<std::string>();
    *timeStamp=id.substr(1,this->current_time_id_len);
    analyse_35_struct.time=this->get_timestamp(*timeStamp);

    analyse_35_struct.user=new id_info(this->analyse_02(id.substr(1+this->current_time_id_len,1+this->current_time_id_len+std::to_string(this->current_user).length())));

    analyse_35_struct.space_id=new id_info(this->analyse_02(id.substr(2+(this->current_time_id_len*2)+std::to_string(this->current_user).length())));
    
    return analyse_35_struct;
};

#endif