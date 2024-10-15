#pragma once

#include <iostream>
#include <string>
#include <cctype>
#include <memory>
#include <functional>

#include <compressor.h>

using namespace std;


class P_probe_class{
    private:
    shrink_operands compressor;
    const short keyLength_min_lim=8;
    const short keyLength_max_lim=16;

    public:
    std::hash<std::string> hasher;
    bool db_checks(std::string key, std::string dataBase_key);
    bool length_check(std::string key);
    bool validity(std::string key);
} P_probe;


bool P_probe_class::length_check(std::string key){

    bool func_say;

    if(key.length() >= keyLength_min_lim && key.length() <= keyLength_max_lim){

        func_say=true;

        } else{

            func_say=false;

        };

    return func_say;
};



//pass in compressed and hashed key into key and database hashed password into database_hashed_key as params.
bool P_probe_class::db_checks(std::string key,std::string dataBase_hashed_key){
    bool hasher_check;

    if(key.size()==dataBase_hashed_key.size() && key==dataBase_hashed_key){

        hasher_check=true;

    } else{

        hasher_check=false;

    };

    return hasher_check;
};




bool P_probe_class::validity(std::string key){

std::unique_ptr<bool> up_c=std::make_unique<bool>(false);
std::unique_ptr<bool> low_c=std::make_unique<bool>(false);
std::unique_ptr<bool> digit=std::make_unique<bool>(false);
std::unique_ptr<bool> special_char=std::make_unique<bool>(false);

    if(this->length_check(key)){

        for(short i=0;i<key.length();i++){

            std::isupper(key[i])?*up_c=true:
            std::islower(key[i])?*low_c=true:
            std::isdigit(key[i])?*digit=true:
            std::ispunct(key[i])?*special_char=true:*special_char=false;
            
        };

    };

    return (up_c || low_c && digit && special_char);
};