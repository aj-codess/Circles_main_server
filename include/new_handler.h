#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>

using namespace std;

class handler{
    public:
    std::string gen_new();

    private:

};


std::string handler::gen_new(){

    auto current_time=std::chrono::system_clock::now();

    auto get_duration=current_time.time_since_epoch();

    auto get_in_milli=std::chrono::duration_cast<std::chrono::milliseconds>(get_duration).count();

    return (std::to_string(get_in_milli));
};