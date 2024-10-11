#pragma once

#include <iostream>
#include <string>

#include <jsonScript.h>

using namespace std;

class log_controller{
    public:
    void mail_pass_checks(login_init initials,std::function<void(bool)> callback);

    private:

};


void log_controller::mail_pass_checks(login_init initials,std::function<void(bool)> callback){

    if(initials.bio_data=="" && initials.phone==""){

        //pass gmail and pass into their respective sector

    } else{

    };

};