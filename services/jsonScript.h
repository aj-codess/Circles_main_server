#pragma once

#include <iostream>
#include <string>
#include <functional>

#include <nlohmann/json.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>

using namespace std;
struct login_init{
    std::string gmail="";
    std::string password="";
    std::string phone="";
    std::string bio_data="";

    login_init()=default;
};

class jsonScript{
    private:

    public:
    jsonScript()=default;
    void id_conv(boost::beast::http::response<boost::beast::http::string_body>& res,std::string target,std::function<void(std::string)> callback);
    void bool_conv(boost::beast::http::response<boost::beast::http::string_body>& res,std::string target,std::function<void(bool)> callback);
    login_init conv_log_data(boost::beast::http::request<boost::beast::http::string_body> res);
    nlohmann::json bool_json(std::string ref,bool t_f);
    std::string push_getter(boost::beast::http::request<boost::beast::http::string_body> req);
    
} script;


void jsonScript::id_conv(boost::beast::http::response<boost::beast::http::string_body>& res,std::string target,std::function<void(std::string)> callback){

    try{
        nlohmann::json json_body=nlohmann::json::parse(res.body());

        if(json_body.find(target)==json_body.end()){
            cout<<"id not found"<<endl;
        } else{
            if(callback){

                std::string id=json_body[target].get<std::string>();

                callback(id);

            };
        };

    } catch(const std::exception& e){
        cout<<"Error Parsing - "<<e.what();
    };

};



void jsonScript::bool_conv(boost::beast::http::response<boost::beast::http::string_body>& res,std::string target,std::function<void(bool)> callback){
    
    try{
        nlohmann::json json_body=nlohmann::json::parse(res.body());

        if(json_body.find(target)==json_body.end()){

            cout<<"target not found"<<endl;

        } else{

          if(callback){

            bool deleted=json_body[target].get<bool>();

            callback(deleted);

          };  

        };

    } catch(const exception& e){
        cout<<"Error Parsing - "<<e.what()<<endl;
    };

};



login_init jsonScript::conv_log_data(boost::beast::http::request<boost::beast::http::string_body> req){

    login_init log_info;

    nlohmann::json json_body=nlohmann::json::parse(req.body());

    if(json_body.find("gmail") != json_body.end() && json_body.find("password") != json_body.end()){

        log_info={json_body["gmail"].get<std::string>(),json_body["password"].get<std::string>(),"",""};

    } else if(json_body.find("phone") != json_body.end() && json_body.find("bio_data") != json_body.end()){

        log_info={"","",json_body["phone"].get<std::string>(),json_body["bio_data"].get<std::string>()};

    };

    return log_info;
}; 



nlohmann::json jsonScript::bool_json(std::string ref,bool t_f){
    
    nlohmann::json body={
        {ref,t_f}
    };

    return body;
};


std::string jsonScript::push_getter(boost::beast::http::request<boost::beast::http::string_body> req){

    std::string token;

    nlohmann::json json_body=nlohmann::json::parse(req.body());

    if(json_body.find("push_token") != json_body.end()){

        token=json_body["push_token"].get<std::string>();

    };

    return token;
};