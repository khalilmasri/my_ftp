#ifndef DATA_HPP
#define DATA_HPP

#include "server.hpp"
#include "ftp.hpp"

// LIBRARIES
#include <thread>
#include <iostream>
#include <fstream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <map>
#include <vector>
#include <sstream>
#include <random>
#include <string>
#include <algorithm>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>



class Data 
{
private:

    Server data_server;
    
    // Client data transfer
    int data_port;
    std::string data_ip;
    int data_socket;

    int server_sock;
    bool passive_mode;

    std::string filepath;

    void sendData(std::string);


public:
    Data();
    ~Data();

    int connectData(int, std::string);
    bool listHandle(std::string, std::string);
    bool retrHandle(std::string);
    bool pasvHandle(int, std::string);
};

#endif
