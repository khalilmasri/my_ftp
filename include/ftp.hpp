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
        std::string path;
        int dataPort;


        // Origin client details
        int request_id;
        struct sockaddr_in request_addr;
        char buff[MAX_TRANSMISSION_LENGTH];
        socklen_t addr_size = sizeof(request_addr);
        sockaddr_in sendSockAddr;

        // state and authorization
        int current_state = 0;
        bool authorized = false;

        // User data
        std::string user;
        std::string pass;

        // File handle
        DIR *dir;
        dirent *current_dir;
        std::vector<std::string> file_list;

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
        void sendMsg(const int, std::string info);

        // Command handlers
        void userHandle();
        void passHandle();
        void pasvHandle();
        void getpwdHandle();
        void getcwdHandle();
        void getcdupHandle();
        void listHandle();
        void unvalidCommand();
        void quitHandle();

        std::map<std::string, void (Ftp::*)()> dispatch_table{
            {"USER", &Ftp::userHandle},
            {"SYST", &Ftp::unvalidCommand},
            {"PASS", &Ftp::passHandle},
            {"PASV", &Ftp::pasvHandle},
            {"PWD", &Ftp::getpwdHandle},
            {"CWD", &Ftp::getcwdHandle},
            {"CDUP", &Ftp::getcdupHandle},
            {"LS", &Ftp::listHandle},
            {"QUIT", &Ftp::quitHandle}
        };

        std::vector<std::string> commands = {"USER", "PWD", "CDUP", "CWD", "PASS", "PASV", "LS", "LPRT", "SYST", "QUIT"};

        // Server reply and status
        std::map<int, std::string> server_reply{
            {150 , "150 - File status okay; about to open data connection."},
            {220 , "220 - Service ready for new user."},
            {221 , "221 - Service closing."},
            {226 , "226 - send ok"},
            {227 , "227 - Entering passive mode"},
            {230 , "230 - User logged in successfully."},
            {250 , "250 - Requested file action okay, completed."},
            {331 , "331 - User name okay, need a password."},
            {332 , "332 - Need account for login."},
            {500 , "500 - Syntax error, unkown command."},
            {501 , "501 - Syntax error in parameters or arguments."},
            {502 , "502 - Command not implemented."},
            {530 , "530 - User password is wrong, didn't login."},
            {550 , "550 - Requested action not taken. File unavailable."}
        };

    public:
        Ftp();

};

#endif
