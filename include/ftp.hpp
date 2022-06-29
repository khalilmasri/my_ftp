#ifndef FTP_HPP
#define FTP_HPP

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
#include <algorithm>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include "server.hpp"

extern Server server;
extern bool detach;

class Ftp{

    private:

        // SERVER DETAILS
        int server_sock;
        int port;
        std::string filePath;
        int dataPort;
        std::string server_ip;

        // Origin client details
        int request_id;
        struct sockaddr_in request_addr;
        socklen_t addr_size = sizeof(request_addr);
        sockaddr_in sendSockAddr;

        // Data socket details
        int data_id;
        struct sockaddr_in data_addr;
        socklen_t data_addr_size = sizeof(data_addr);

        // state and authorization
        bool pasv_mode = false;
        bool authorized = false;

        // User data
        std::string user;
        std::string pass;

        // Request handlers
        void getRequest();

        // Input handlers
        std::vector<std::string> input;
        void parseCommand();
        void handleCommand();
        void handle_request();

    public:
        Ftp();

        int getServerSock() const;
        int getServerPort() const;
        std::string getServerIP() const;
        bool getPASV() const;
        std::string getFilePath() const;
        int getDataPort() const;

        int getRequestID() const;
        bool getAuth() const;
        int getDataID() const;

        void setUser(const std::string);
        void setPass(const std::string);
        void setDataPort(const int);
        
        void setPASV(const bool);
        void setAuth(const bool);

        std::string getUser() const;
        std::string getPass() const;

        bool listenRequest();
        int listenData(int);
};

#endif
