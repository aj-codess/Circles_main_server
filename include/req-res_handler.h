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

using namespace std;

class req_res_handler{
    public:
    void structure(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res);

    private:
    bool path_vir(boost::beast::http::request<boost::beast::http::string_body>& req,std::string path);
};


void req_res_handler::structure(boost::beast::http::request<boost::beast::http::string_body>& req,boost::beast::http::response<boost::beast::http::string_body>& res){


    res.keep_alive(req.keep_alive());

    res.prepare_payload();
};



bool path_vir(boost::beast::http::request<boost::beast::http::string_body>& req,std::string path){
    boost::urls::url_view url(req.target());

    std::string url_path=std::string(url.encoded_path());

    return (url_path==path);
};