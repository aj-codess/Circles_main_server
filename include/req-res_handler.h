#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <functional>

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
    bool token_vir(std::string token,std::function<void(std::string)> callback);
    jsonScript script;
    log_controller controller;
    handler foreign_token;
    std::string id_getter(std::string token);
};


void req_res_handler::structure(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res){

    res.version(req.version());
    
    res.set(boost::beast::http::field::server, "Circujoin");

    res.set(boost::beast::http::field::content_type, "application/json");

        std::string token;

        auto extract_token=[&](std::function<void(std::string)> callback){

            if(req.find(boost::beast::http::field::authorization) != req.end()){

                std::string aut=req[boost::beast::http::field::authorization];

                if (aut.find("Bearer ") == 0){

                    if(callback){
                        callback(aut.substr(7));
                    };

                } else if(aut.find("Pre_Bearer ")==0){

                    if(callback){
                        callback(aut.substr(11));
                    };

                };

            } else if(req.find(boost::beast::http::field::authorization) == req.end()){

                std::string foreigner_token=this->generate_token(this->foreign_token.gen_new());

                req.set(boost::beast::http::field::authorization,"Pre_Bearer "+foreigner_token);

                res.set(boost::beast::http::field::authorization,"Pre_Bearer "+foreigner_token);

                if(callback){
                    callback(foreigner_token);
                };

            };

        };

        extract_token([&](std::string ach_token){

                token=ach_token;
            
        });


    auto checks=[&](std::string tkn,std::function<void()> callback){

        bool isToken;

        std::function<void(std::string)> empty_callback;

        isToken=this->token_vir(tkn,empty_callback);

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

                    std::string auth=req[boost::beast::http::field::authorization];

                        std::string id;
                        
                        this->token_vir(auth.substr(11),[&](std::string token_id){

                            id=token_id;

                        });

                        if(this->foreign_token.exists(id)==false){

                            this->foreign_token.setter(id,log_data);

                        };

                    res.body()=this->script.bool_json("isCreated",isPassed);

                } else{

                    res.body()=this->script.bool_json("isCreated",isPassed);

                };

            });

        } else if(this->path_vir(req,"/signup/make_push")){

            res.set(boost::beast::http::status::ok);

            res.set(boost::beast::http::field::content_type,"application/json");

            std::string auth=req[boost::beast::http::field::authorization];

            if(auth.find("Pre_Bearer ") != std::string::npos){

                std::string id;

                this->token_vir(auth.substr(11),[&](std::string token_id){

                    id=token_id;

                });

                if(this->foreign_token.exists(id)==true){

                    std::string option=this->script.option_conv(req.body());

                    user_recog client_data=this->foreign_token.logger[id];
                    
                    //work on option_conv grabbing the push option
                    //pass option to the domain probe push option 
                    //genetate and store token in the the logger map with regards to the id 
                    //keep a timer 
                    //make a submit push route grabbing the id from authorization find time difference and validate it
                    //make and set a real user id 
                    //get initials, store in data base and clear from the logger map

                };

            };

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



bool req_res_handler::token_vir(std::string token,std::function<void(std::string)> callback){

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

        if(callback){
            callback(tokenGen_user_id);
        };

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



