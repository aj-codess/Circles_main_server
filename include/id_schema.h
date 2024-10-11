#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <memory>

#include <id_gen_connector.h>
#include <jsonScript.h>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>

using namespace std;

  struct id_info{
        std::string useCase;
        std::string time;
        std::string pos_on_server;
        id_info* user;
        id_info* space_id;
    };


    struct persistent{
        uint64_t current_user_n;
        uint64_t current_space_n;
        uint64_t current_time_len;
    };


class id_schema{
    public:
    uint64_t user_space_position(bool user_or_space);
    std::string generate_id(std::string option,std::string user_id="0",std::string space_id="0");
    bool del_user(id_info id_struct);//requires reader call
    bool del_space(id_info id_struct);//requires reader call
    bool del_ugc(id_info id_struct);//requires reader call
    bool check_existance(std::string id);
    void persistent_init(std::function<void(persistent persist)> callback);

    private:
    id_gen_con id_server;
    jsonScript json_conv;
};



void id_schema::persistent_init(std::function<void(persistent persist)> callback){

    payload_opt payload_req={true,false};

    this->id_server.io_pipe(payload_req,[&](boost::beast::http::response<boost::beast::http::string_body> res){

        try{
            persistent persist;

            nlohmann::json json_body=nlohmann::json::parse(res.body());

            if(json_body.find("current_user_n") != json_body.end() && json_body.find("current_space_n") != json_body.end() && json_body.find("current_time_len") != json_body.end()){
                persist.current_user_n=json_body["current_user_n"].get<uint64_t>();
                persist.current_space_n=json_body["current_space_n"].get<uint64_t>();
                persist.current_time_len=json_body["current_time_len"].get<short>();
            };

            if(callback){

                callback(persist);

            };

        } catch(const std::exception& e){

            cout<<"Error Parsing - "<<e.what()<<endl;

        };

    });

};


string id_schema::generate_id(std::string option,std::string user_id,std::string space_id){
    std::string id;

    payload_opt payload_req={false,false,option,user_id,space_id, };

    this->id_server.io_pipe(payload_req,[&](boost::beast::http::response<boost::beast::http::string_body> res){

        this->json_conv.id_conv(res,"id", [&](std::string parsed_id){

            id=parsed_id;

        });

    });

    return id;
};



//requires reader call
bool id_schema::del_user(id_info id_struct){
    bool isDeleted;

    std::string user_id=id_struct.useCase+id_struct.time+id_struct.pos_on_server;

    std::string server_position=id_struct.pos_on_server;

    payload_opt payload_req={false,true,nullptr,user_id,nullptr,nullptr,"user",server_position};

    this->id_server.io_pipe(payload_req,[&](boost::beast::http::response<boost::beast::http::string_body> res){

        this->json_conv.bool_conv(res,"isDeleted",[&](bool deleted){

            isDeleted=deleted;

        });

    });
    
    return isDeleted;
};



//requires reader call
bool id_schema::del_space(id_info id_struct){
    bool isDeleted;

    std::string space_id=id_struct.useCase+id_struct.time+id_struct.pos_on_server;

    std::string server_position=id_struct.pos_on_server;

    payload_opt payload_req={false,true,nullptr,nullptr,space_id,nullptr,"space",nullptr,server_position};

    this->id_server.io_pipe(payload_req,[&](boost::beast::http::response<boost::beast::http::string_body> res){

        this->json_conv.bool_conv(res,"isDeleted",[&](bool deleted){

            isDeleted=deleted;

        });

    });

    return isDeleted;
};



//requires reader call
bool id_schema::del_ugc(id_info id_struct){
    bool isDeleted;

    std::string user_id=id_struct.user->useCase+id_struct.user->time+id_struct.user->pos_on_server;

    std::string ugc_id=id_struct.useCase+id_struct.time;

    payload_opt payload_req={false,true,nullptr,user_id,nullptr,ugc_id,"ugc"};

    this->id_server.io_pipe(payload_req,[&](boost::beast::http::response<boost::beast::http::string_body> res){

        this->json_conv.bool_conv(res,"isDeleted",[&](bool deleted){

            isDeleted=deleted;

        });

    });

    return isDeleted;
};



bool id_schema::check_existance(std::string id){
    bool isFound;

    payload_opt payload_req={};

    return isFound;
};