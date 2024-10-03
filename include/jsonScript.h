#pragma once

#include <iostream>
#include <string>

#include <nlohmann/json.hpp>
#include <boost/asio.hpp>
#include <boost/beast.hpp>

using namespace std;

class jsonScript{
    private:

    public:
    std::string id_conv(boost::beast::http::response<boost::beast::http::string_body>& res,std::function<void(std::string)> callback);
    std::string bool_conv(boost::beast::http::response<boost::beast::http::string_body>& res,std::function<void(bool)> callback);
};


std::string jsonScript::id_conv(boost::beast::http::response<boost::beast::http::string_body>& res,std::string target,std::function<void(std::string)> callback){
    std::string to_return;

    try{
        nlohmann::json json_body=nlohmann::json::parse(res.body());

        if(json_body.find(target)==json_body.end()){
            cout<<"id not found"<<endl;
        } else{
            if(callback){
                callback(json_body[target]);
            };
        };

    } catch(const std::exception& e){
        cout<<"Error Parsing - "<<e.what();
    };

    return to_return;
};


bool jsonScript::bool_conv(boost::beast::http::response<boost::beast::http::string_body>& res,std::string target,std::function<void(bool)> callback){
    bool returner;

    try{
        nlohmann::json json_body=nlohmann::json::parse(res.body());

        if(json_body.find(target)==json_body.end()){

            cout<<"target not found"<<endl;

        } else{

          if(callback){
            callback(json_body[target]);
          };  

        };

    } catch(const exception& e){
        cout<<"Error Parsing - "<<e.what()<<endl;
    };

    return returner;
};