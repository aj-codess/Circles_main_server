#pragma once

#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <array>
#include <thread>
#include <memory>
#include <functional>

using namespace std;

class domain_probe{
    public:
    bool mail_validity(std::string mail_addr);
    void push_override(std::string mail_addr,std::string phone,std::string token_ext,std::function<void(bool)> callback);
    std::string token_generator();

    private:
    const int token_length=5;
    const double delay=120;
    const string second_level_domain[3]={"gmail","yahoo","hotmail"};
    const string top_level_domain=".com";
    const char concatinator='@';
};


std::string domain_probe::token_generator(){    

    std::string token;

    std::vector<int> gen_holder;

    std::mt19937 generator(std::random_device{}());

    std::uniform_int_distribution<int> distribution(0,9);

        for(int i=0;i<this->token_length;i++){

                gen_holder.push_back(distribution(generator));
                
                token+=std::to_string(gen_holder.at(i));

        };

    return token;
};



bool domain_probe::mail_validity(std::string mail_addr){

    bool isValid;

    auto prefix_checks=[this](std::string mail,std::function<void(bool)> callback){

        bool mail_domain,internet_domain,concat;

        for(int i=0;i<3;i++){

            if(mail.find(this->second_level_domain[i]) != std::string::npos){

                mail_domain=true;

            };

        };


            if(mail.find(this->concatinator) != std::string::npos){

                concat=true;

            } else{
                concat=false;
            };

            if(mail.find(this->top_level_domain) != std::string::npos){

                internet_domain=true;

            } else{
                internet_domain=false;
            };


        if(callback){

            if(mail_domain==true && internet_domain==true && concat==true){

                callback(true);

            } else{
                
                callback(false);

            };

        };

    };

    prefix_checks(mail_addr,[&](bool passed){

        if(passed==true){

            isValid=true;

        }else{

            isValid=false;

        };

    });

    return isValid;
};



void domain_probe::push_override(std::string mail_addr,std::string phone,std::string token_ext,std::function<void(bool)> callback){

    bool isPushed;

    if(mail_addr.size()>1){

        if(this->mail_validity(mail_addr)==true){

            isPushed=true;

            //make mail push

        } else{

            isPushed=false;

        };

    } else if(phone.size()>1){
        //make the push for phone.
    } else{

        isPushed=false;

    };


    if(callback){
        callback(isPushed);
    };

};
