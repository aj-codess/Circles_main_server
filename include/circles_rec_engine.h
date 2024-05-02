#ifndef DOMAIN_RECOMMENDATION_ENGINE
#define DOMAIN_RECOMMENDATION_ENGINE

#define SPICE 5
#define T_PERCENT 100

#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <iterator>
#include <cctype>
#include <limits>

using namespace std;

class circles_rec_module{
    private:
    const short limit=std::numeric_limits<short>::max()-SPICE;
    void safety(std::string id,std::string topic);
    std::string toUp(std::string text);
    void update_in_data(std::string id);

    public:
    bool id_finder(std::string id);
    bool update_topic(std::string id,std::string topic_2_updt,int rating,bool uprate);
    bool delete_topic(std::string id,std::string topic_2_delete);
    bool clear_circle_d(std::string id);//triggers when space is deleted
    std::string gen_pref(std::string id);

} circles_rec;



struct in_data{
    double current;
    double perc;
};



std::map<std::string,std::map<std::string,in_data>> rec_struc;



bool circles_rec_module::id_finder(std::string id){
    std::unique_ptr<bool> isFound=std::make_unique<bool>();

    auto itr=rec_struc.find(id);

    if(itr!=rec_struc.end()){
        *isFound=true;
    } else{
        *isFound=false;
    };

  return *isFound;  
};




std::string circles_rec_module::toUp(std::string text){
    std::unique_ptr<std::string> text_2_up=std::make_unique<std::string>();

    for(char t:text){
        *text_2_up+=std::toupper(t);
    };

    return *text_2_up;
};




void circles_rec_module::safety(std::string id,std::string topic){

    if(rec_struc[id][topic].current >= this->limit){

        rec_struc[id][topic].current = this->limit;

    };

};




bool circles_rec_module::update_topic(std::string id,std::string topic_2_updt,int rating,bool uprate){

    std::unique_ptr<bool> isUpdated=std::make_unique<bool>();

    if(this->id_finder(id)==true){
      auto itr=rec_struc[id].find(this->toUp(topic_2_updt));

      if(itr!=rec_struc[id].end() && uprate==true){

        rec_struc[id][this->toUp(topic_2_updt)].current+=rating;
        *isUpdated=true;

      } else if(itr!=rec_struc[id].end() && uprate==false){

        rec_struc[id][this->toUp(topic_2_updt)].current-=rating;
        *isUpdated=true;

      } else{

        rec_struc[id][this->toUp(topic_2_updt)].current=1;

        *isUpdated=true;

      };

    this->safety(id,this->toUp(topic_2_updt));
    
    this->update_in_data(id);

    } else{

      *isUpdated=false;

    };


  return *isUpdated;
};




bool circles_rec_module::delete_topic(std::string id,std::string topic_2_delete){
   std::unique_ptr<bool> isDeleted=std::make_unique<bool>();

  if(this->id_finder(id)==true){
    auto itr=rec_struc[id].find(this->toUp(topic_2_delete));

    if(itr!=rec_struc[id].end()){

      rec_struc[id].erase(this->toUp(topic_2_delete));
      *isDeleted=true;

    } else{
      *isDeleted=false;
    };

    this->update_in_data(id);

  } else{
    *isDeleted=false;
  };

    return *isDeleted;
};




bool circles_rec_module::clear_circle_d(std::string id){
    std::unique_ptr<bool> isDeleted=std::make_unique<bool>();

    if(this->id_finder(id)==true){
        rec_struc.erase(id);
    };

    this->id_finder(id)==true?*isDeleted=false:*isDeleted=true;

    return *isDeleted;
};



void circles_rec_module::update_in_data(std::string id){

  std::unique_ptr<int> T_current_per_id=std::make_unique<int>();

  for(auto itr=rec_struc[id].begin();itr!=rec_struc[id].end();itr++){
      *T_current_per_id+=rec_struc.at(id).at(itr->first).current;
  };


  for(auto itr=rec_struc[id].begin();itr!=rec_struc[id].end();itr++){
    rec_struc[id][itr->first].perc=(rec_struc[id][itr->first].current / *T_current_per_id)*(T_PERCENT);
  };

};




std::string circles_rec_module::gen_pref(std::string id){
  std::unique_ptr<std::string> pref=std::make_unique<std::string>();

  std::unique_ptr<short> holder=std::make_unique<short>(0);

    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(1,100);

    short guess=distribution(generator);

    for(auto itr=rec_struc[id].begin();itr!=rec_struc[id].end();itr++){
      *holder+=rec_struc[id][itr->first].perc;
      
      if(guess<=*holder){
        *pref=itr->first;
        break;
      };

    };

  return *pref;
};


#endif