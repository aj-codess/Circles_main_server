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

class operands{

    public:
    short t_rating;
    int seed=0;
    string read_t;
    short t_per_page;
    int read_t_index;
    vector<std::string> global_t;
    vector<int> weights;
    string stability();
    void global_checks();
    void insert();
    void weight_checks();
    bool finder();
    vector<std::string> consumer;
    vector<std::string> t_2_query;
    void push_operator(int no_2_itr,string t_2_itr);
    void query_dataBase();
    int index;
    void consumer_writer();
    string high_o;
    void most_occuring();
    bool full;
  } global_t_operands;


  bool operands::finder(){
    std::unique_ptr<bool> t_f=std::make_unique<bool>();

    std::unique_ptr<int> get_size=std::make_unique<int>(this->global_t.size());

    for(int i=0;i<*get_size;i++){

      std::regex to_find (this->global_t.at(i));

      if(std::regex_match(this->read_t,to_find)){
        this->read_t_index=i;
        *t_f=true;
      } else{
        *t_f=false;
      }
    };
    return *t_f;
  };


  string operands::stability(){

      std::unique_ptr<int> get_x_size=std::make_unique<int>(this->global_t.size());
      std::unique_ptr<int> get_y_size=std::make_unique<int>(this->weights.size());

      if(*get_x_size==*get_y_size){
        return "stable";
      } else{
        return "sever Crashed";
      }
  };

  
  void operands::global_checks(){
    this->finder()?this->weight_checks():this->insert();
  };


  void operands::insert(){
    this->global_t.push_back(this->read_t);
    this->weights.push_back(1);
  };


  void operands::weight_checks(){
    this->weights.at(this->read_t_index)+=this->t_rating;
  };


void operands::push_operator(int no_2_itr,string t_2_itr){

  int i=0;

  while(i<no_2_itr){
    this->t_2_query.push_back(t_2_itr);
    i++;
  };
};


  void operands::query_dataBase(){

      if(this->stability()=="stable"){
        std::unique_ptr<int> x=std::make_unique<int>(this->global_t.size());

        for(int i=0;i<*x;i++){

          string get_t=this->global_t.at(i);
          int get_t_repeat=this->weights.at(i);
          this->push_operator(get_t_repeat,get_t);
        };

        x.reset();
      } else{
        this->stability();
        this->query_dataBase();
      };
      this->consumer_writer();
  };



void operands::most_occuring(){

  std::unique_ptr<int> get_size=std::make_unique<int>(this->weights.size());

  if(*get_size >= 3){
        if(this->index != (*get_size -2)){
      if(this->weights.at(this->index) < this->weights.at(this->index+1)){
        this->high_o=this->global_t.at(this->index+1);
      } else{
        this->high_o=this->global_t.at(this->index);
      };
      this->index+=1;
      this->most_occuring();
    };
  } else{
    this->high_o=this->global_t.at(0);
  }

  get_size.reset();
};



void operands::consumer_writer(){

  if(full){
    this->query_dataBase();
    this->full=false;
  };

  std::unique_ptr<int> get_size=std::make_unique<int>(this->t_2_query.size());

  if(*get_size < this->t_per_page){

    this->most_occuring();

    for(int i=this->seed;i<this->t_per_page;i++){
      t_2_query.push_back(this->high_o);
    };

    this->full=true;
  };

  for(int i=this->seed;i<this->t_2_query.size() && i<this->t_per_page;i++){
    this->consumer.push_back(this->t_2_query.at(i));
  };
  this->seed=t_per_page;
  this->t_per_page+=this->seed;

  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(this->consumer.begin(), this->consumer.end(), g);

};

#endif