#ifndef CREATOR
#define CREATOR

#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <algorithm>
#include <chrono>
#include <thread>

using namespace std;


struct shield_data{
    bool shield_ini;
    std::string owner_long;
    std::string owner_lat;
    bool isOpen;
    bool man_aut;
    bool timer_open;
};


struct data_2_hold{
    std::string ownership_id;
    std::vector<std::string> sub_owners;
    std::string loc_longitude;
    std::string loc_latitude;
    std::vector<std::string> space_piece;
    std::string space_name;
    shield_data log_info;
    std::vector<std::string> awaiting_space_piece;
    std::vector<std::string> space_pref;
};


map<std::string,data_2_hold> space_containers;


class creator_operands{
    public:
    bool create(std::string id,std::string ownership_id,std::string space_name,std::string owner_long_lat[],bool options[]);
    bool exists(std::string space_id,std::string space_name);
    bool name_checker(std::string space_name);
    bool join(std::string space_id,std::string user_id);
    bool delete_space(std::string space_id,std::string user_id);
    bool open_space(std::string space_id,std::string user_id,int timer);
    std::vector<std::string> get_awaiting(std::string space_id,std::string owners_id);
    std::vector<std::string> get_space_piece(std::string space_id);
    bool accept_in_space(std::string space_id,std::string owners_id,std::string sub_admin,std::string awaiting_user);
    bool make_admin(std::string space_id,std::string owners_id,std::string user_2_admin_id);
    bool remove_admin(std::string space_id,std::string owners_id,std::string admin_2_remove_id);
    bool add_2_space(std::string space_id,std::string owners_id,std::string sub_admin,std::string id_2_add);
    bool remove_piece(std::string space_id,std::string owners_id,std::string sub_admin,std::string id_2_remove);
    bool update_pref(std::string space_id);

    private:
    std::vector<std::string> name_pool;
} creator;


bool creator_operands::create(std::string id,std::string ownership_id,std::string space_name,std::string owner_long_lat[],bool options[]){
    std::unique_ptr<bool> isCreated=std::make_unique<bool>();

    if(exists(id,space_name)==true){
        *isCreated=false;
    } else{

        space_containers[id]=data_2_hold();
        space_containers[id].ownership_id=ownership_id;
        space_containers[id].space_name=space_name;
        this->name_pool.push_back(space_name);
        // space_containers[id].loc_latitude;
        // space_containers[id].loc_latitude;
        // long and lat of the location where space was created
        space_containers[id].log_info.owner_long=owner_long_lat[0];
        space_containers[id].log_info.owner_lat=owner_long_lat[1];
        space_containers[id].log_info.shield_ini=options[0];
        space_containers[id].log_info.isOpen=options[1];
        space_containers[id].log_info.man_aut=options[2];
        space_containers[id].log_info.timer_open=options[3];

        *isCreated=true;
    };

    return *isCreated;
};



bool creator_operands::name_checker(std::string space_name){
    std::unique_ptr<bool> name_exists=std::make_unique<bool>();

    auto itr=std::find(name_pool.begin(),name_pool.end(),space_name);

    if(itr != name_pool.end()){
        *name_exists=true;
    } else{
        *name_exists=false;
    }

    return *name_exists;
};



bool creator_operands::exists(std::string space_id,std::string space_name=""){
    std::unique_ptr<bool> exists=std::make_unique<bool>();

    if(space_containers.find(space_id) != space_containers.end() || name_checker(space_name)==true){
        *exists=true;
    } else{
        *exists=false;
    };

    return *exists;
};




bool creator_operands::join(std::string space_id,std::string user_id){
    std::unique_ptr<bool> isJoined=std::make_unique<bool>();

    if(space_containers.find(space_id) != space_containers.end() && space_containers[space_id].log_info.isOpen == true){

        space_containers[space_id].space_piece.push_back(user_id);
        *isJoined=true;

    } else if(space_containers.find(space_id) != space_containers.end() && space_containers[space_id].log_info.timer_open==true){

        space_containers[space_id].awaiting_space_piece.push_back(user_id);
        *isJoined=true;

    } else if(space_containers.find(space_id) != space_containers.end() && space_containers[space_id].log_info.man_aut==true){

        space_containers[space_id].awaiting_space_piece.push_back(user_id);
        *isJoined=true;

    } else {
        *isJoined=false;
    };

    return *isJoined;
};




bool creator_operands::delete_space(std::string space_id,std::string user_id){

    std::unique_ptr<bool> isDeleted=std::make_unique<bool>();

    std::unique_ptr<bool> exists=std::make_unique<bool>();
    *exists=this->exists(space_id);

    if(*exists==true && space_containers[space_id].ownership_id==user_id){

            auto itr=std::find(this->name_pool.begin(),this->name_pool.end(),space_containers[space_id].space_name);

            if(itr != this->name_pool.end()){
                this->name_pool.erase(itr);
            };

            space_containers.erase(space_id);

            *isDeleted=true;
    } else{
        *isDeleted=false;
    };

    return *isDeleted;
};




bool creator_operands::open_space(std::string space_id,std::string user_id,int timer){
    std::unique_ptr<bool> isOpen=std::make_unique<bool>();

if(this->exists(space_id) && user_id==space_containers[space_id].ownership_id){

        space_containers[space_id].log_info.timer_open=true;

        std::this_thread::sleep_for(std::chrono::seconds(timer));
};

    space_containers[space_id].log_info.timer_open=false;

    return *isOpen;
};




std::vector<std::string> creator_operands::get_awaiting(std::string space_id,std::string owners_id){
    std::unique_ptr<std::vector<std::string>> awaiting_list=std::make_unique<std::vector<std::string>>();

    if(this->exists(space_id)==true && space_containers[space_id].ownership_id==owners_id){
        *awaiting_list=space_containers[space_id].awaiting_space_piece;
    };

    return *awaiting_list;
};



std::vector<std::string> creator_operands::get_space_piece(std::string space_id){
    std::unique_ptr<std::vector<std::string>> pieces=std::make_unique<std::vector<std::string>>();

    if(this->exists(space_id)==true){
        *pieces=space_containers[space_id].space_piece;
    };

    return *pieces;
};




bool creator_operands::accept_in_space(std::string space_id,std::string owners_id,std::string sub_admin,std::string awaiting_user){
    std::unique_ptr<bool> isAccepted=std::make_unique<bool>();

        auto checks=std::find(space_containers[space_id].sub_owners.begin(),space_containers[space_id].sub_owners.end(),sub_admin);

    if(this->exists(space_id)==true && space_containers[space_id].ownership_id==owners_id || checks!=space_containers[space_id].sub_owners.end()){
        auto itr=std::find(space_containers[space_id].awaiting_space_piece.begin(),space_containers[space_id].awaiting_space_piece.end(),awaiting_user);

        if(itr==space_containers[space_id].awaiting_space_piece.end()){
            *isAccepted=false;
        } else{
            space_containers[space_id].space_piece.push_back(awaiting_user);
            space_containers[space_id].awaiting_space_piece.erase(itr);
            *isAccepted=true;
        };

    };

    return *isAccepted;
};




bool creator_operands::make_admin(std::string space_id,std::string owners_id,std::string user_2_admin_id){
    std::unique_ptr<bool> isSubAdmin=std::make_unique<bool>();

    if(this->exists(space_id)==true && space_containers[space_id].ownership_id==owners_id){
        auto itr=std::find(space_containers[space_id].space_piece.begin(),space_containers[space_id].space_piece.end(),user_2_admin_id);

        if(itr!=space_containers[space_id].space_piece.end()){
            space_containers[space_id].sub_owners.push_back(user_2_admin_id);
            *isSubAdmin=true;
        } else{
            *isSubAdmin=false;
        };

    } else{
        *isSubAdmin=false;
    };

    return *isSubAdmin;
};




bool creator_operands::remove_admin(std::string space_id,std::string owners_id,std::string admin_2_remove_id){
    std::unique_ptr<bool> isRemoved=std::make_unique<bool>();

    if(this->exists(space_id)==true && space_containers[space_id].ownership_id==owners_id){

        auto itr=std::find(space_containers[space_id].sub_owners.begin(),space_containers[space_id].sub_owners.end(),admin_2_remove_id);

        if(itr!=space_containers[space_id].space_piece.end()){
            space_containers[space_id].sub_owners.erase(itr);
            *isRemoved=true;
        } else{
            *isRemoved=false;
        };
    }

    return *isRemoved;
};



bool creator_operands::add_2_space(std::string space_id,std::string owners_id,std::string sub_admin,std::string id_2_add){
    std::unique_ptr<bool> isAdded=std::make_unique<bool>();

    auto checks=std::find(space_containers[space_id].sub_owners.begin(),space_containers[space_id].sub_owners.end(),id_2_add);

    if(this->exists(space_id)==true && space_containers[space_id].ownership_id==owners_id || checks!=space_containers[space_id].sub_owners.end()){
        space_containers[space_id].space_piece.push_back(id_2_add);
        *isAdded=true;
    } else{
        *isAdded=false;
    };

    return *isAdded;
};



bool creator_operands::remove_piece(std::string space_id,std::string owners_id,std::string sub_admin,std::string id_2_remove){
    std::unique_ptr<bool> isRemoved=std::make_unique<bool>();

    auto itr=std::find(space_containers[space_id].sub_owners.begin(),space_containers[space_id].sub_owners.end(),sub_admin);

    if(this->exists(space_id)==true && space_containers[space_id].ownership_id==owners_id || itr!=space_containers[space_id].sub_owners.end()){
        auto checks=std::find(space_containers[space_id].space_piece.begin(),space_containers[space_id].space_piece.end(),id_2_remove);

        if(checks!=space_containers[space_id].space_piece.end()){
            
            space_containers[space_id].space_piece.erase(checks);
            *isRemoved=true;

        } else{
            *isRemoved=false;
        };
        
    } else{
        *isRemoved=false;
    }

    return *isRemoved;
};



bool creator_operands::update_pref(std::string space_id){
    std::unque_ptr<bool> isUpdated=std::make_unique<bool>();

    if(this->exists(space_id)==true){
        *isUpdated=false;
    }

    return *isUpdated;
};



#endif