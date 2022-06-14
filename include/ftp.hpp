#ifndef FTP_HPP
#define FTP_HPP

// LIBRARIES
#include <thread>
#include <iostream>
#include <fstream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <map>
#include <vector>

#include "server.hpp"

extern int server_sock;

#define MAX_TRANSMISSION_LENGTH 100000

class Ftp{

    private:

        // Origin client details
        int request_id;
        struct sockaddr_in request_addr;
        char buff[MAX_TRANSMISSION_LENGTH];
        socklen_t addr_size = sizeof(request_addr);

        // state and authorization
        int current_state = 0;
        bool authorized = false;

        // User data
        std::string user;
        std::string pass;

        // Request handlers
        void listen_request();
        void getRequest();

        // Input handlers
        std::vector<std::string> input;
        void parseCommand();
        void handleCommand();
        void handle_request();

        // Command handlers
        void user_handle();
        void pass_handle();
        void pasv_handle();
        void quit_handle();

        std::map<std::string, void (Ftp::*)()> dispatch_table{
            {"USER", &Ftp::user_handle},
            {"PASS", &Ftp::pass_handle},
            {"PASV", &Ftp::pasv_handle},
            {"QUIT", &Ftp::quit_handle}
        };

        std::vector<std::string> commands = {"USER", "PASS", "PASV", "QUIT"};

        // Server reply and status
        std::map<int, std::string> server_reply{
                {220 , "Service ready for new user."},
                {221 , "Service closing."},
                {227 , "Entering passive mode"},
                {226 , "send ok"},
                {230 , "User looged in, proceed."},
                {331 , "User name okay, need a password."},
                {332 , "Need account for login."},
                {500 , "Syntax error, unkown command."}
        };

    public:
        Ftp();
};

#endif
