#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include <thread>

#include <log_controller.h>
#include <jsonScript.h>
#include <new_handler.h>
#include <domain_probe.h>
#include <space_c_engine.h>

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
    req_res_handler() = default; 
    void structure(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res,boost::beast::tcp_stream& con_socket);
    bool initial;

    private:
    bool path_vir(boost::beast::http::request<boost::beast::http::string_body>& req,std::string path);
    std::string generate_token(std::string user_id);
    bool token_vir(std::string token,std::function<void(std::string)> callback);
    std::string id_getter(std::string token);
    std::string session_timer;
    void session_del_controller();
    std::atomic<bool> thread_running=false;
};



void req_res_handler::structure(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res,boost::beast::tcp_stream& con_socket){

    if(this->initial==true){

        this->session_timer=foreigner.gen_new();

        this->initial=false;

    };


    if(foreigner.time_diff(foreigner.time_conv(foreigner.gen_new()),foreigner.time_conv(this->session_timer)) > 180 ) {

        this->session_timer=foreigner.gen_new();

        std::thread deletion_thread(&req_res_handler::session_del_controller, this);

        deletion_thread.detach();

    };

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

                std::string foreigner_token=this->generate_token(foreigner.gen_new());

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



    auto checks=[&](std::string tkn,std::function<void(std::string)> callback){

        bool isToken;

        std::function<void(std::string)> empty_callback;

        std::string id;

        isToken=this->token_vir(tkn,[&](std::string user_id){

            id=user_id;

        });

        if(isToken==true){

            if(callback){

                callback(id);

            };

        } else{ 

            res.body()="Error with session ";

        };

    };



    checks(token,[&](std::string id){

        switch(req.method()){

        case boost::beast::http::verb::get:
            
            if(this->path_vir(req,"/world")==true){

                res.result(boost::beast::http::status::ok);

                //check if id exist in the db and send its content

                //if id does not exist then create the session for the client with the map based in the id
                if(/*if id is not in db &&*/foreigner.exists(id)==false){
                    //give limited access to client
                    foreigner.creator(id);

                } else if(/*if id is not in db &&*/ foreigner.exists(id)==true){

                    // give limited access to client

                } 
                // else if(/*if id is in db*/){

                //     //give full access to the client
                //     //give feedback based on what is in client db

                // }

                res.body()="get the fuck otta here ";

            } else{

                res.result(boost::beast::http::status::not_found);

                res.body()="Get Resource not Found";

            };

        break;


        case boost::beast::http::verb::post:
 
        if(this->path_vir(req,"/signup")){

            res.set(boost::beast::http::field::content_type,"application/json");

            controller.mail_pass_checks(script.conv_log_data(req),[&](login_init log_data,bool isPassed){

                res.result(boost::beast::http::status::ok);

                if(isPassed==true){

                        if(foreigner.exists(id)==false){

                            foreigner.setter(id,log_data);

                        };

                    res.body()=script.bool_json("isCreated",isPassed);

                } else{

                    res.body()=script.bool_json("isCreated",isPassed);

                };

            });


        } else if(this->path_vir(req,"/signup/make_push")){

            bool isPushed;

            res.result(boost::beast::http::status::ok);

            res.set(boost::beast::http::field::content_type,"application/json");

                if(foreigner.exists(id)==true){

                    log_data id_content=foreigner.logger[id];

                    std::string token=domain_handler.token_generator();

                    domain_handler.push_override(id_content.initials.gmail,id_content.initials.phone,token,[&](bool pushed){

                        isPushed=pushed;

                    });

                    foreigner.set_time_token(id,foreigner.gen_new(),token);

                };

            res.body()=script.bool_json("Push_init",isPushed);


        } else if(this->path_vir(req,"signup/push_submit")){
            
            res.result(boost::beast::http::status::ok);

            res.set(boost::beast::http::field::content_type,"application/json");

            bool isSaved;

            if(foreigner.exists(id)==true){

                std::string push_token=script.push_getter(req);

                log_data id_content=foreigner.logger[id];

                foreigner.submit_override(id_content,push_token,[&](bool isPassed){

                    if(isPassed==true){

                        if(/*if gmail and phone already exist*/true){

                            //get the id associated with the gmail and make a jwt with it and set it to the auth
                            // std::string token=this->generate_token(/*id from db*/);

                            //res.set(boost::beast::http::field::authorization,"Bearer "+token);

                        } else{

                            std::string new_id=id_server.generate_id("0","0","0");

                            std::string token=this->generate_token(new_id);

                            res.set(boost::beast::http::field::authorization,"Bearer "+token);

                            //get initials, store in data base and clear from the logger map - id_content variable

                            //save data to db - id_content variable

                        };

                        isSaved=true;

                    } else{

                        isSaved=false;

                    };

                });

            } else{

                res.body()="Error with session";

            };

            res.body()=script.bool_json("isLogged",isSaved);

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




void req_res_handler::session_del_controller(){

    if(this->thread_running.exchange(true)) return;

    try{
        cout<<"new thread started"<<endl;

        foreigner.session_del_override();

        cout<<"Session cleaned"<<endl;

    } catch(const std::exception e){

        cout<<"Error with session cleaner - "<<e.what()<<endl;

    };

    this->thread_running=false;

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

//the below condition is very necessary but it is ignored for the sake of testing
        // if(tokenGen_user_id==db_id){

        //     isUser=true;

        // } else{

        //     isUser=false;

        // };

        isUser=true;

    } catch (const std::exception& e) {

        std::cerr << "Token verification failed: " << e.what() << std::endl;

        isUser=false;
    };

    return isUser;

};