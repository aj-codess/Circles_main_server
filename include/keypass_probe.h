#ifndef KEYPASS
#define KEYPASS

#include <iostream>
#include <string>
#include <cctype>
#include <memory>
#include <functional>

using namespace std;


class P_probe_class{
    private:
    const short keyLength_min_lim=8;
    const short keyLength_max_lim=16;

    public:
    std::hash<std::string> hasher;
    std::string key;
    string second_opt_key;
    bool hash_checks(std::string key, std::string dataBase_key);
    bool comparism();
    bool length_check();
    bool matcher(std::string key,std::string dataBase_hash_key);
    bool validity();
} P_probe;


bool P_probe_class::length_check(){
    bool* func_say=new bool;
    if(this->key.length() >= this->keyLength_min_lim && this->key.length() <= this->keyLength_max_lim){
        *func_say=true;
        } else{
            *func_say=false;
        };
    return *func_say;
    delete func_say;
};



bool P_probe_class::hash_checks(std::string key,std::string dataBase_hash_key){
    bool* hasher_check=new bool;

    if(key.size()==dataBase_hash_key.size() && key==dataBase_hash_key){
        *hasher_check=true;
    } else{
        *hasher_check=false;
    };
    return *hasher_check;
    delete hasher_check;
};



bool P_probe_class::comparism(){
    bool* say=new bool;
        this->key==this->second_opt_key?*say=true:*say=false;
        return *say;
        delete say;
};




bool P_probe_class::matcher(std::string key,std::string hash_key){
    bool* func_say=new bool;

this->hash_checks(key,hash_key)==true?*func_say=true:*func_say=false;
    return *func_say;
    delete func_say;
};




bool P_probe_class::validity(){

std::unique_ptr<bool> up_c=std::make_unique<bool>(false);
std::unique_ptr<bool> low_c=std::make_unique<bool>(false);
std::unique_ptr<bool> digit=std::make_unique<bool>(false);
std::unique_ptr<bool> special_char=std::make_unique<bool>(false);

    if(this->length_check()){

        for(short i=0;i<this->key.length();i++){

            std::isupper(this->key[i])?*up_c=true:
            std::islower(this->key[i])?*low_c=true:
            std::isdigit(this->key[i])?*digit=true:
            std::ispunct(this->key[i])?*special_char=true:
            *special_char=false;
            
        };

    };

    return (up_c || low_c && digit && special_char);
};



#endif