#pragma once

#include <iostream>
#include <string>
#include <memory>

#include <log_controller.h>
#include <jsonScript.h>
#include <new_handler.h>

#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/coroutine/all.hpp>
#include <boost/beast.hpp>
#include <boost/url.hpp>
#include <nlohmann/json.hpp>
#include <id_schema.h>
#include <fstream>

#include <jwt-cpp/jwt.h>

using namespace std;

class req_res_handler{
    public:
    void structure(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res);

    private:
    bool path_vir(boost::beast::http::request<boost::beast::http::string_body>& req,std::string path);
    std::string generate_token(std::string user_id);
    bool token_vir(std::string token);
    jsonScript script;
    log_controller controller;
    handler foreign_token;
};


void req_res_handler::structure(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res){

    res.version(req.version());
    
    res.set(boost::beast::http::field::server, "Circujoin");

    res.set(boost::beast::http::field::content_type, "application/json");

        std::string token;

        auto extract_token=[&](std::function<void(std::string)> callback){

            if(req.find(boost::beast::http::field::authorization) != req.end()){

                std::string aut=req[boost::beast::http::field::authorization];

                if (aut.find("Bearer ") == 0) {

                    if(callback){
                        callback(aut.substr(7));
                    };

                } else{

                    //create the session token

                    res.result(boost::beast::http::status::forbidden);

                    res.body()="session Lost.";

                };

            } else if(req.find(boost::beast::http::field::authorization) == req.end()){

                //make a spare token for the user

            };

        };

        extract_token([&](std::string ach_token){

            token=ach_token;
            
        });


    auto checks=[&](std::string tkn,std::function<void()> callback){

        bool isToken;

        isToken=this->token_vir(tkn);

        if(isToken==true){

            if(callback){
                callback();
            };

        } else{

            res.body()="Error with session ";

        };

    };

    checks(token,[&](){

        switch(req.method()){

        case boost::beast::http::verb::get:
            
            if(this->path_vir(req,"/")==true){

                res.result(boost::beast::http::status::ok);

                res.body()="get the fuck outahere";

            } else{

                res.result(boost::beast::http::status::not_found);

                res.body()="Resource not Found";

            };

        break;





        case boost::beast::http::verb::post:

        if(this->path_vir(req,"/signup")){

            res.set(boost::beast::http::field::content_type,"application/json");

            this->controller.mail_pass_checks(this->script.conv_log_data(req),[&](login_init log_data,bool isPassed){

                res.result(boost::beast::http::status::ok);

                if(isPassed==true){

                    //pass the log_data to the database

                    res.body()=this->script.bool_json("isCreated",isPassed);

                }

            });

        };
            
        break;

        case boost::beast::http::verb::delete_:
            
        break;

        case boost::beast::http::verb::put:
          
        break;

        case boost::beast::http::verb::options:
           
        break;

        case boost::beast::http::verb::head:
            
        break;

        case boost::beast::http::verb::connect:
            
        break;

        default:
        res.result(boost::beast::http::status::bad_request);

        res.body()="invalid request method";

        break;
    };

    });

    res.keep_alive(req.keep_alive());

    res.prepare_payload();
};



bool req_res_handler::path_vir(boost::beast::http::request<boost::beast::http::string_body>& req,std::string path){
    boost::urls::url_view url(req.target());

    std::string url_path=std::string(url.encoded_path());

    return (url_path==path);
};



std::string req_res_handler::generate_token(std::string user_id){

    std::string token;

    auto env_read=[&](std::function<void(std::string)> callback) {

        std::ifstream file("./include/env.txt");

        std::stringstream buffer;

        buffer << file.rdbuf();

        if(callback){

            callback(buffer.str());

        };

    };

    env_read([&](std::string key_value){

        auto gen_token=jwt::create().set_issuer("auth0").set_type("JWT").set_payload_claim("user_id",jwt::claim(user_id)).sign(jwt::algorithm::hs256{key_value});

        token=gen_token;

    });

    return token;

};



bool req_res_handler::token_vir(std::string token){

    bool isUser=false;

    try {

        std::ifstream file("./include/env.txt");

        std::stringstream buffer;

        buffer << file.rdbuf();

        auto decoded = jwt::decode(token);

        auto verifier = jwt::verify()
                          .allow_algorithm(jwt::algorithm::hs256{buffer.str()})
                          .with_issuer("auth0");

        verifier.verify(decoded);

        std::string tokenGen_user_id = decoded.get_payload_claim("user_id").as_string();

        //check for the existence of tokenGen_user_id and compare it to the generated token

        std::string db_id;

        if(tokenGen_user_id==db_id){

            isUser=true;

        } else{

            isUser=false;

        };

    } catch (const std::exception& e) {

        std::cerr << "Token verification failed: " << e.what() << std::endl;

        isUser=false;
    };

    return isUser;

};