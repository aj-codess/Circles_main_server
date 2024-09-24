#pragma once

#include <iostream>
#include <string>
#include <memory>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/url.hpp>


class url_handler{
    private:

    public:
    bool path_vir(boost::beast::http::request<boost::beast::http::string_body>& req,std::string path);
};


bool path_vir(boost::beast::http::request<boost::beast::http::string_body>& req,std::string path){
    boost::urls::url_view url(req.target());

    std::string url_path=std::string(url.encoded_path());

    return (url_path==path);
};