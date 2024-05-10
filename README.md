For contributors.
after cloning this repository, using vcpkg strictly, install boost-asio and crow c++ libries.
using the below repository, see how to use vcpkg to install new packages and use them.
https://github.com/aj-codess/cpp-setting-out-and-adding-packages-with-vcpkg

run the CMakeLists.txt to generate a build or buid files.

main.cpp being the entry code whiles main being the executable

other dependencies found in the ./include directory




--------------------gmail verification handler-----------------------------
testing unit code below for reference

// dont touch this blog

    // E_probe_class E_probe;
    // std::unique_ptr<bool> email_probe_handler_result=std::make_unique<bool>(false); // holds the entire email checks results


    // *email_probe_handler_result=E_probe.validator();//mail address goes into the braces


    // E_probe.push_option;/*push option in here. 0 for phone and 1 for gmail push*
    // E_probe.phone_no;/*phone number in here. stringifies*/

    // std::promise<bool> probe_promise;
    // std::future<bool> probe_promise_holder=probe_promise.get_future();


    // std::thread veri_checks_thread([&E_probe,&probe_promise](){

    //     std::unique_ptr<bool> veri_checksResult=std::make_unique<bool>(false);
    //     *veri_checksResult=E_probe.veri_checks();
    //     probe_promise.set_value(*veri_checksResult);

    // });


    // while(E_probe.met_condition!=true){
    //     E_probe.returned_token; // returned token in here
    //     std::this_thread::sleep_for(std::chrono::seconds(3));
    // };

    // veri_checks_thread.join();

    // *email_probe_handler_result=probe_promise_holder.get();





----keypass_probe.h----
// documentaztion
P_probe.key; -  equate this to a string of password for validity
P_probe.second_opt_key - equate  this to a string of second password input
P_probe.validity(); // for character set comparism

//if validity is true then check for length doing
cout<<"for length check"<<P_probe.length_check()<<endl;//if true then proceed to the next 

cout<<"for two input comparism"<<P_probe.comparism()<<endl;// for the two input box comparism.


//for dataBase comparism....
cout<<"checking if they are the sam "<<P_probe.matcher(hasher(compressor(string variable to compare with dataBase)),string of dataBase hash)<<endl;



----hasher.h----
string message;
// cout<<"encrypted hash Message "<<hasher(shrink.compress(message))<<endl;

hasher function accepts string of compressed junk



----Task associated with id_schema.h----

std::string user_id=schema.generate_id("0","0","0");

std::string space_id=schema.generate_id("2","0","0");

cout<<"user - "<<schema.generate_id("0","0","0")<<endl;
cout<<"ugc - "<<schema.generate_id("1",user_id,"0")<<endl;
cout<<"space - "<<schema.generate_id("2","0","0")<<endl;
cout<<"space ugc - "<<schema.generate_id("3",user_id,space_id)<<endl;
cout<<"chat - "<<schema.generate_id("4",user_id,"0")<<endl;
cout<<"space chat - "<<schema.generate_id("5",user_id,space_id)<<endl;


----Task associated with id_reader.h----
std::string space_ugc_id=schema.generate_id("3",schema.generate_id("0","0","0"),schema.generate_id("2","0","0"));

cout<<id_reader_handler.parser(id,false).dump(2)<<endl;
cout<<"id to be read  - "<<space_ugc_id<<endl;

false being passed as an argument is used to specify if the passed in id which is an argument is to be deleted or not.
if True meaning the id is to be deleted and if false meaning the id is not to be deleted.



----Task associated with space_c_engine.h----
    std::string space_id=schema.generate_id("2","0","0"); - //space id for container reference
    std::string user_id=schema.generate_id("0","0","0"); - //space owner id
    std::string space_name="telecom_campus base"; -  // specifying space name
    std::string owner_long_lat[2]; -             // specifying the location of the owner where space is created - array of longitude and latitude
    bool options[4]={false,false,false,false};  - // array of shield options 0-initialise shield, 1-open space , 2-manual authentication when joining space, 3-timer option where space will be open within a time range

    cout<<space_creator(space_id,user_id,space_name,owner_long_lat,options)<<endl; - for creating new space 
    cout<<creator.delete_space(space_id,user_id)<<endl; - for deleting space
    cout<<creator.join(space_id,user_id)<<endl; - for joining a particular space where user_id is the id of the user joining




----------- timer related type of open space and how it is being tested ----------
    std::string space_id=schema.generate_id("2","0","0");
    std::string space_admin=schema.generate_id("0","0","0");
    std::string owner_long_lat[2];
    bool options[3]={true,false,true};
    space_creator(space_id,space_admin,"telecom_campus base",owner_long_lat,options); // original function for creating spaces

// this is a different blog....................
    std::promise<bool> space_o_result;
    std::future<bool> result_holder=space_o_result.get_future();

    creator_operands creator;

    std::thread open_space_thread([&creator,&space_o_result,&space_id,&space_admin](){
        std::unique_ptr<bool> isOpen=std::make_unique<bool>();
        *isOpen=creator.open_space(space_id,space_admin,60);
        space_o_result.set_value(*isOpen);
    });

    for(int i=0;i<100;i++){
        std::string user_id=schema.generate_id("0","0","0");
        creator.join(space_id,user_id);
    };

    open_space_thread.join();

    cout<<"space still open ?...."<<result_holder.get()<<endl;
    
    cout<<"list of awaiting space piece - "<<get_awaiting(space_id,space_admin).dump(2)<<endl;
    cout<<"list of joined members - "<<get_piece(space_id).dump(2)<<endl;
    cout<<"admin of space - ref - space admin variable "<<space_admin<<endl;
    cout<<"admin of space - ref - space data check "<<creator.get_space_owners_id(space_id)<<endl;



------get awaiting users------
function will be returned in json
get_awaiting(space_id,owners_id)
the above is the function and its arguments it takes which is strictly string.


-----get space piece-------
this function get the users found in a particular space which returns in json
get_piece(space_id);


----accept in space------
this is a function which is used when man_aut is true.
to call - creator.accept_in_space(space_id,owners_id,sub_admin,awaiting_user)
where all arguments are strings, and awaiting_user is the user intended to add to the space from temporary add.



----make_admin funtion----
make_admin(space_id,owners_id,id_2_make_admin)
this function takes all string argument which is the space id, admin id or owners id as well as id to make admin.


---remove admin function----
creator.remove_admin(space_id,owners_id,admin_2_remove_id);


----add_2_space function-----
creator.add_2_space(space_id,owners_id,sub_admin,id_2_add);


----remove_piece function----
creator.remove_piece(space_id,owners_id,sub_admin,id_2_remove);


---------
in all where sub_admin is any admin aside the main admin or the space owner.





------updating space preference------
this is done by calling
creator.update_pref(space_id,"-preference-")




----recommendation engine-----
  bool id_finder(std::string id_2_find);//check for the availability of an id or client
  bool update_topic(std::string id,std::string topic_2_updt,int rating,bool uprate);//update topic
  bool delete_topic(std::string id,std::string topic_2_delete);//not interested topic
  bool clear_deleted_user_d(std::string id_2_clear);//clear deleted accound data
  bool update_f_new(std::string id,std::vector<std::string> new_list);//update pref for new users
  std::string gen_pref(std::string id);//gets preference based on the statistics

    std::vector<std::string> subjects = {"Mathematics", "Physics", "Chemistry", "Biology", "Computer Science"};
    cout<<rec_operands.update_f_new(user_id,subjects)<<endl;
    cout<<rec_operands.gen_pref(user_id)<<endl;










    
