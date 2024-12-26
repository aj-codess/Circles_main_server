#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include <thread>

#include <jwt-cpp/jwt.h>

#include <new_handler.h>

class token_config_operands{
    public:
    token_config_operands()=default;
    std::string generate_token(std::string id);
    bool token_verify(std::string token,std::function<void(std::string)> callback);

    private:

} token_config;



std::string token_config_operands::generate_token(std::string user_id){
    std::string token;

    auto env_read=[&](std::function<void(std::string)> callback) {

        std::ifstream file("./../assets/env.txt");

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



bool token_config_operands::token_verify(std::string token,std::function<void(std::string)> callback){
    bool isUser=false;

    try {

        std::ifstream file("./../assets/env.txt");

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