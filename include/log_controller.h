#pragma once

#include <iostream>
#include <string>

#include <jsonScript.h>
#include <domain_probe.h>
#include <keypass_probe.h>

using namespace std;

class log_controller{
    public:
    void mail_pass_checks(login_init initials,std::function<void(login_init,bool)> callback);

    private:
    domain_probe mail_checks;
    P_probe_class pass_checks;
};



void log_controller::mail_pass_checks(login_init initials,std::function<void(login_init,bool)> callback){

    bool isPassed;

    if(initials.bio_data=="" && initials.phone==""){

        if(this->mail_checks.mail_validity(initials.gmail)==true){

            if(this->pass_checks.validity(initials.password)==true){

                isPassed=true;

            } else if(this->pass_checks.validity(initials.password)==false){

                    isPassed=false;

            };

        } else{

            isPassed=false;

        };

    } else if(initials.password=="" && initials.gmail==""){

        

    };

    if(callback){
        callback(initials,isPassed);
    };

};