#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include <thread>

#include <token_extractor.h>
#include <log_controller.h>
#include <jsonScript.h>
#include <new_user_controller.h>
#include <domain_probe.h>
#include <space_c_engine.h>
#include <login_routes.h>

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
    void structure(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res,boost::beast::tcp_stream& con_socket,std::function<void()> callback);
    bool initial;

    private:
    log_routes log_handler;
    bool path_vir(boost::beast::http::request<boost::beast::http::string_body>& req,std::string path);
    std::string generate_token(std::string user_id);
    bool token_vir(std::string token,std::function<void(std::string)> callback);
    std::string id_getter(std::string token);
    std::string session_timer;
    void session_del_controller();
    std::atomic<bool> thread_running=false;
};



void req_res_handler::structure(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res,boost::beast::tcp_stream& con_socket,std::function<void()> callback){

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

    token_ext.token_middler(req,res,[&](std::string id){

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