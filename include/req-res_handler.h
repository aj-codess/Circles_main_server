#pragma once

#include <iostream>
#include <string>
#include <memory>

#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/coroutine/all.hpp>
#include <boost/beast.hpp>
#include <boost/url.hpp>
#include <nlohmann/json.hpp>

#include <url_handler.h>

using namespace std;

class req_res_handler{
    public:
    void structure(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res);

    private:
    url_handler url;
};


void req_res_handler::structure(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res){

    res.version(req.version());
    
    res.set(boost::beast::http::field::server, "Circujoin");

    res.set(boost::beast::http::field::content_type, "text/html");

    switch(req.method()){

        case boost::beast::http::verb::get:
            
            
        break;

        case boost::beast::http::verb::post:
            
        break;

        case boost::beast::http::verb::delete_:
            
        break;

        case boost::beast::http::verb::put:
          
        break;

        case boost::beast::http::verb::options:
           
        break;

        case boost::beast::http::verb::head:
            
        break;

        case boost::beast::http::verb::connect:
            
        break;

        default:
        res.result(boost::beast::http::status::bad_request);

        res.body()="invalid request method";

        break;
    };

    res.keep_alive(req.keep_alive());

    res.prepare_payload();
};