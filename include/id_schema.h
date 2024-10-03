#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <memory>
#include <vector>

#include <id_gen_connector.h>
#include <jsonScript.h>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

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
    jsonScript json_conv;
} schema;

//update current and unoccupied from dataabase.


string id_schema::generate_id(std::string option,std::string user_id,std::string space_id){
    std::string id;

    payload_opt payload_req={false,false,option,user_id,space_id, };

    this->id_server.io_pipe(payload_req,[&](boost::beast::http::response<boost::beast::http::string_body> res){

        this->json_conv.id_conv(res,"id",[&](std::string parsed_id){
            id=parsed_id;
        });

    });

    return id;
};



bool id_schema::del_user(std::string user_id,std::string user_pos){
    bool isDeleted;

    payload_opt payload_req={false,true, ,user_id};

    this->id_server.io_pipe(payload_req,[&](boost::beast::http::response<boost::beast::http::string_body> res){

        this->json_conv.id_conv(res,"isDeleted",[&](std::string parsed_data){
            isDeleted=parsed_data;
        })

    });
    
    return isDeleted;
};


bool id_schema::del_space(std::string space_id,std::string space_pos){
    bool isDeleted;

    payload_opt payload_req={false,true, , ,space_id};

    this->id_server.io_pipe(payload_req,[&](boost::beast::http::response<boost::beast::http::string_body> res){

        this->json_conv.id_conv(res,"isDeleted",[&](std::string parsed_data){
            isDeleted=parsed_data;
        });

    });

    return isDeleted;
};


bool id_schema::del_ugc(std::string user_id,std::string ugc_id){
    bool isDeleted;

    payload_opt payload_req={false,true, ,user_id, ,ugc_id};

    this->id_server.io_pipe(payload_req,[&](boost::beast::http::response<boost::beast::http::string_body> res){

        this->json_conv.id_conv(res,"isDeleted",[&](std::string parsed_data){
            isDeleted=parsed_data;
        })

    });

    return isDeleted;
};