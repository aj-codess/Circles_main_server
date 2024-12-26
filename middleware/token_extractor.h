#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <functional>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <fstream>

#include <token_config.h>

class extractor_operands{
public:
    extractor_operands()=default;
    void token_middler(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res,std::function<void(std::string)> callback);

private:
    void token_extract(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res,std::function<void(std::string)> callback);
    void token_checks(std::string token,std::function<void(std::string)> callback);

} token_ext;


void extractor_operands::token_middler(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res,std::function<void(std::string)> callback){

    std::string token;

    this->token_extract(req,res,[&](std::string achieved_token){

        token=achieved_token;

    });


    this->token_checks(token,[&](std::string id){
    
        if(id.size()>0 && callback){

            return callback(id);

        } else{
            return;
        }

    });

};


void extractor_operands::token_extract(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res,std::function<void(std::string)> callback){
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

                std::string foreigner_token=token_config.generate_token(foreigner.gen_new());

                req.set(boost::beast::http::field::authorization,"Pre_Bearer "+foreigner_token);

                res.set(boost::beast::http::field::authorization,"Pre_Bearer "+foreigner_token);

                if(callback){
                    callback(foreigner_token);
                };

            };
};




void extractor_operands::token_checks(std::string token,std::function<void(std::string)> callback){

        bool isToken;

        std::function<void(std::string)> empty_callback;

        std::string id;

        isToken=token_config.token_verify(token,[&](std::string user_id){

            id=user_id;

        });

        if(isToken==true){

            if(callback){

                callback(id);

            };

        } else{ 

            if(callback){

                callback("");

            }

        };

};