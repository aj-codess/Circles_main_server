#ifndef RECOMMENDATION_ALGO
#define RECOMMENDATION_ALGO

#define RANGE 5
#define T_PERCENT 100

// based on your selected topics

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include <random>
#include <memory>
#include <limits>

using namespace std;

class recommendation_operands{
  private:
  const short limit=std::numeric_limits<short>::max()-RANGE;
  void safety(std::string id_2_check,std::string topic_2_check);
  void update_in_data(std::string id);

  public:
  bool id_finder(std::string id_2_find);//check for the availability of an id or client
  bool update_topic(std::string id,std::string topic_2_updt,int rating,bool uprate);//update topic
  bool delete_topic(std::string id,std::string topic_2_delete);//not interested topic
  bool clear_deleted_user_d(std::string id_2_clear);//clear deleted accound data
  bool update_f_new(std::string id,std::vector<std::string> new_list);//update pref for new users
  std::string gen_pref(std::string id);//gets preference based on the statistics

} rec_operands;


struct global{
  short current;
  short total_perC;
};

std::map<std::string,map<std::string,global>> id_2_topic;


void recommendation_operands::safety(std::string id_2_check,std::string topic_2_check){
  if(id_2_topic[id_2_check][topic_2_check].current>=this->limit){
    id_2_topic[id_2_check][topic_2_check].current=this->limit;
  };
};


bool recommendation_operands::id_finder(std::string id_2_find){
  std::unique_ptr<bool> isFound=std::make_unique<bool>();

  auto itr=id_2_topic.find(id_2_find);

  if(itr!=id_2_topic.end()){
    *isFound=true;
  } else{
    *isFound=false;
  };

  return *isFound;
};




bool recommendation_operands::update_topic(std::string id,std::string topic_2_updt,int rating,bool uprate){

    std::unique_ptr<bool> isUpdated=std::make_unique<bool>();

    if(this->id_finder(id)==true){
      auto itr=id_2_topic[id].find(topic_2_updt);

      if(itr!=id_2_topic[id].end() && uprate==true){

        id_2_topic[id][topic_2_updt].current+=rating;
        *isUpdated=true;

      } else if(itr!=id_2_topic[id].end() && uprate==false){

        id_2_topic[id][topic_2_updt].current-=rating;
        *isUpdated=true;

      } else{

        id_2_topic[id][topic_2_updt].current=1;

        *isUpdated=true;

      };

    this->safety(id,topic_2_updt);
    
    this->update_in_data(id);

    } else{

      *isUpdated=false;

    };


  return *isUpdated;
};




bool recommendation_operands::delete_topic(std::string id,std::string topic_2_delete){
   std::unique_ptr<bool> isDeleted=std::make_unique<bool>();

  if(this->id_finder(id)==true){
    auto itr=id_2_topic[id].find(topic_2_delete);

    if(itr!=id_2_topic[id].end()){

      id_2_topic[id].erase(topic_2_delete);
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





bool recommendation_operands::clear_deleted_user_d(std::string id_2_clear){
  std::unique_ptr<bool> Udata_is_cleared=std::make_unique<bool>();

  if(this->id_finder(id_2_clear)==true){

    id_2_topic.erase(id_2_clear);

  };

  this->id_finder(id_2_clear)==false?*Udata_is_cleared=true:*Udata_is_cleared=false;

  return *Udata_is_cleared;
};




void recommendation_operands::update_in_data(std::string id){
  std::unique_ptr<bool> isUpdated=std::make_unique<bool>();

  std::unique_ptr<int> T_current_per_id=std::make_unique<int>();

  for(auto itr=id_2_topic[id].begin();itr!=id_2_topic[id].end();itr++){
      *T_current_per_id+=id_2_topic.at(id).at(itr->first).current;
  };

  for(auto itr=id_2_topic[id].begin();itr!=id_2_topic[id].end();itr++){
    id_2_topic.at(id).at(itr->first).total_perC=(id_2_topic.at(id).at(itr->first).current / *T_current_per_id)*(T_PERCENT);
  };

};



bool recommendation_operands::update_f_new(std::string id,std::vector<std::string> new_list){
    std::unique_ptr<bool> listAppended=std::make_unique<bool>();

    if(this->id_finder(id) != true){

      for(int i=0;i<new_list.size();i++){
        id_2_topic.at(id).at(new_list.at(i)).current=1;
      };

      *listAppended=true;

    } else{

      *listAppended=false;

    };

    this->update_in_data(id);

  return *listAppended;
};



std::string recommendation_operands::gen_pref(std::string id){
  std::unique_ptr<std::string> pref=std::make_unique<std::string>();

  std::unique_ptr<short> holder=std::make_unique<short>(0);

    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(1,100);

    short guess=distribution(generator);

    for(auto itr=id_2_topic[id].begin();itr!=id_2_topic[id].end();itr++){
      *holder+=id_2_topic.at(id).at(itr->first).total_perC;

      if(guess<=*holder){
        *pref=itr->first;
        break;
      };
      
    };


  return *pref;
};


#endif