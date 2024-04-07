#ifndef HASHER
#define HASHER
#define CONST_PREFIX ".Space"

#include <iostream>
#include <string>
#include <functional>
#include <memory>

using namespace std;

class hasher_class{
    private:
    string addons(string key);
    std::hash<std::string> hasher;
    
    public:
    string hash(string chunk);
} hasher_ref;



string hasher_class::addons(string key){
    std::unique_ptr<string> to_add=std::make_unique<string>(key+CONST_PREFIX);

   return *to_add;
};



string hasher_class::hash(string chunk){

    std::unique_ptr<string> hashed_value=std::make_unique<string>(std::to_string(this->hasher(this->addons(chunk))));
    return *hashed_value;
};

#endif