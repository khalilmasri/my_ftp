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
extern bool detach;

#define MAX_TRANSMISSION_LENGTH 1500

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

        // Message sender
        void sendMsg(const int);

        // Command handlers
        void user_handle();
        void pass_handle();
        void pasv_handle();
        void list_handle();
        void quit_handle();

        std::map<std::string, void (Ftp::*)()> dispatch_table{
            {"USER", &Ftp::user_handle},
            {"PASS", &Ftp::pass_handle},
            {"PASV", &Ftp::pasv_handle},
            {"LIST", &Ftp::list_handle},
            {"QUIT", &Ftp::quit_handle}
        };

        std::vector<std::string> commands = {"USER", "PASS", "PASV", "LIST", "QUIT"};

        // Server reply and status
        std::map<int, std::string> server_reply{
            {150 , "File status okay; about to open data connection."},
            {220 , "Service ready for new user."},
            {221 , "Service closing."},
            {227 , "Entering passive mode"},
            {226 , "send ok"},
            {230 , "User logged in successfully."},
            {331 , "User name okay, need a password."},
            {332 , "Need account for login."},
            {500 , "Syntax error, unkown command."},
            {530 , "User password is wrong, didn't login."}
        };

    public:
        Ftp();
};

#endif
