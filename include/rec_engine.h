#ifndef RECOMMENDATION_ALGO
#define RECOMMENDATION_ALGO

// based on your selected topics

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <random>
#include <memory>

using namespace std;

class recommendation_operands{
  private:
  short limit=100;
  void safety(std::string id_2_check,std::string topic_2_check);
  void update_in_data(std::string id);

  public:
  bool id_finder(std::string id_2_find);//check for the availability of an id or client
  bool update_topic(std::string id,std::string topic_2_updt,int rating,bool uprate);//update topic
  bool delete_topic(std::string id,std::string topic_2_delete);//not interested topic
  bool clear_deleted_user_d(std::string id_2_clear);//clear deleted accound data

} rec_operands;


struct global{
  short current;
  short total_perC;
};

std::map<std::string,map<std::string,global>> id_2_topic;


void recommendation_operands::safety(std::string id_2_check,std::string topic_2_check){
  id_2_topic[id_2_check][topic_2_check].current>this->limit?id_2_topic[id_2_check][topic_2_check].current=this->limit:nullptr;
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

            this->safety(id,topic_2_updt);

        id_2_topic[id][topic_2_updt].current+=rating;
        *isUpdated=true;

      } else if(itr!=id_2_topic[id].end() && uprate==false){

        id_2_topic[id][topic_2_updt].current-=rating;
        *isUpdated=true;

      } else{

        id_2_topic[id][topic_2_updt].current=1;
        *isUpdated=true;

      };

    };

    update_in_data(id);

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

  } else{
    *isDeleted=false;
  };

      update_in_data(id);

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



#endif