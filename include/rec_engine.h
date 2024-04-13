#ifndef RECOMMENDATION_ALGO
#define RECOMMENDATION_ALGO

// based on your selected topics

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <iterator>
#include <random>
#include <memory>

using namespace std;

class recommendation_operands{
  private:

  public:
  bool id_finder(std::string id_2_find);
  bool update_topic(std::string id,std::string topic_2_updt,int rating);

} rec_operands;


std::map<std::string,int> topic_2_ratings;
std::map<std::string,topic_2_ratings> id_2_topic;




bool recommendation_operands::id_finder(std::string id_2_find){
  std::unique_ptr<bool> isFound=std::make_unique<bool>();

  auto itr=id_2_topic.find(id_2_find);

  if(itr!=id_2_topic.end()){
    isFound=true;
  } else{
    isFound=false;
  };

  return *isFound;
};




bool recommendation_operands::update_topic(std::string id,std::string topic_2_updt,int rating){
    std::unique_ptr<bool> isUpdated=std::make_unique<bool>();


    if(this->id_finder(id_2_make)==true){
      auto itr=id_2_topic[id].find(topic_2_updt);

      if(itr!=id_2_topics[id].end()){
        id_2_topic[id][topic_2_updt]+=rating;
      } else{
        id_2_topic[id][topic_2_updt]=1;
      };

    };
    

    return *isUpdated;
};






#endif