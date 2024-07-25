#pragma once

#include <iostream>
#include <string>
#include <thread>
#include <memory>

#include <boost/asio.hpp>
#include <server_con_handler.h>

using namespace std;


class thread_handler{
public:
    std::vector<thread> sockets_thread;

};

