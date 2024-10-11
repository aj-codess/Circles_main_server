#pragma once

#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <array>
#include <thread>
#include <memory>

using namespace std;


class domain_probe{
    public:

    private:
    std::string token_generator();
};


std::string domain_probe::token_generator(){

    

}






































// class E_probe_class{
//     private:
//     const double delay=120;//no call
//     const string second_level_domain[3]={"gmail","yahoo","hotmail"}; //no call
//     const string top_level_domain=".com"; //no call
//     const char concatinator='@'; //no call

//     public:
//     short push_option;//0 for phone push and 1 for gmail push
//     std::string token;
//     void push_msg();
//     bool timeout(short counter); // no call
//     string returned_token; //no call
//     string mail_address; //no call
//     string phone_no;//no call
//     const short digit_num=5; //no call
//     bool veri_checks(); 
//     bool met_condition=false;
//     bool concat=false; //no call
//     bool top_level=false; //no call
//     bool second_level=false; //no call
//     void check(string key);
//     bool validator(string key);
// };

//     bool* voice_out=new bool;
//     short* counter_add=new short;
//     std::size_t* get_length=new size_t;



// // alerts out if the counter is equal to the required time
// bool E_probe_class::timeout(short counter){
//     *counter_add+=counter;
    
//     *counter_add>=this->delay?*voice_out=true:*voice_out=false;

//     return *voice_out;
// };



// void E_probe_class::check(string key){
//     key.find(this->concatinator) != std::string::npos?this->concat=true:this->concat=false;
//     key.find(this->top_level_domain) != std::string::npos?this->top_level=true:this->top_level=false;

//     for(short i=0;i<3;i++){
//         if(key.find(this->second_level_domain[i]) != std::string::npos){
//             this->second_level=true;
//             break;
//         } else{
//             this->second_level=false;
//         };
//     }
// };


// bool E_probe_class::validator(string key){
//     this->check(key);

//     std::unique_ptr<bool> passer=std::make_unique<bool>();

//     if(this->concat==true && this->top_level==true && this->second_level==true){
//         *passer=true;
//         this->mail_address=key;
//     } else {
//         *passer=false;
//     };
//     return *passer;
// };




// bool E_probe_class::veri_checks(){

//     std::unique_ptr<bool> valid=std::make_unique<bool>(false);
    
//     std::unique_ptr<int[]> num=std::make_unique<int[]>(this->digit_num);
    
//     std::mt19937 generator(std::random_device{}());
//     std::uniform_int_distribution<int> distribution(0,9);


//     if(this->validator(this->mail_address)==true){

//         for(int i=0;i<this->digit_num;i++){
//         num[i]=distribution(generator);
//          this->token+=std::to_string(num[i]);
//         };

//         auto start{std::chrono::steady_clock::now()};

//         this->push_msg();

//     while(this->timeout(0) != true){
//         this->timeout(7);

//         cout<<"This is the length of the returned token - "<<this->returned_token.length()<<endl;
//         cout<<"This is the returned token  - "<<this->returned_token<<endl;

//         if(this->returned_token.length()==static_cast<std::string::size_type>(this->digit_num)){
//             this->met_condition=true;
//             break;
//         };

//         std::this_thread::sleep_for(std::chrono::seconds(7));
//     };

//         auto end(std::chrono::steady_clock::now());

//         std::chrono::duration<double> round_seconds{end-start};
        

//         if(this->returned_token.length()==this->digit_num && round_seconds.count() <= this->delay && this->token == this-> returned_token){
//             *valid=true;
//         };
//     };

//     cout<<*valid<<endl;

//     return *valid;
//     delete voice_out;
//     delete counter_add;
// };



// void E_probe_class::push_msg(){
//     std::unique_ptr<string> token_2_send=std::make_unique<string>(this->token);
//     std::unique_ptr<string> mail_2_recieve=std::make_unique<string>(this->mail_address);
//     std::unique_ptr<string> phone_2_recieve=std::make_unique<string>(this->phone_no);
    
//     // push message execution will be done here
//     if(this->push_option==1){
//         //phone push;
//         cout<<"phone push notification was called"<<endl;
//         cout<<"this is the token in the push message  - "<<*token_2_send<<endl;
//     } else{
//         //gmail push;
//         cout<<"gmail push notification was called"<<endl;
//     };

// };