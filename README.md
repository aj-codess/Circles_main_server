For contributors.
after cloning this repository, using vcpkg strictly, install boost-asio and crow c++ libries.
using the below repository, see how to use vcpkg to install new packages and use them.
https://github.com/aj-codess/cpp-setting-out-and-adding-packages-with-vcpkg

run the CMakeLists.txt to generate a build or buid files.

main.cpp being the entry code whiles main being the executable

other dependencies found in the ./include directory
Node js sub engine found in the ./sub_engine directory




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
// cout<<"encrypted hash Message "<<hasher(compressor(message))<<endl;

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

cout<<id_reader(space_ugc_id,false).dump(2)<<endl;
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
    std::promise<bool> get_test;
    std::future<bool> get_test_holder=get_test.get_future();

    std::thread testing_thread([&space_operations,&get_test,&space_id,&user_id](){
        std::unique_ptr<bool> isOpen=std::make_unique<bool>();
        *isOpen=creator.open_space(space_id,user_id,10);
        get_test.set_value(*isOpen);
    });

    for(int i=0;i<100;i++){
        creator.join(space_id,user_id);
    };

    testing_thread.join();

    cout<<"space still open ?...."<<get_test_holder.get()<<endl;



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








    
