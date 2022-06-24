
#ifndef REQUEST_HPP
#define REQUEST_HPP

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



class Request : public Ftp
{
private:
    Ftp ftp_com;

    // Client data transfer
    int data_port;
    std::string data_ip;
    int data_socket;

    std::string TMP = ".tmp/";

    int current_state = 0;
    bool passive_mode = false;

    char buff[MAX_TRANSMISSION_LENGTH];

    // Input handlers
    std::vector<std::string> input;
    void parseCommand();
    void handleCommand();
    void connectBack();

    // Message sender
    void sendMsg(const int);
    void sendMsg(const int, std::string address);
    void sendData(const int, std::string data);

    // Command handlers
    void userHandle();
    void passHandle();
    void pasvHandle();
    void listHandle();
    void portHandle();
    void getpwdHandle();
    void getcwdHandle();
    void getcdupHandle();
    void retrHandle();
    void unvalidCommand();
    void quitHandle();

    // File handle
    DIR *dir;
    dirent *current_dir;
    std::vector<std::string> file_list;

    std::map<std::string, void (Request::*)()> dispatch_table{
        {"USER", &Request::userHandle},
        {"SYST", &Request::unvalidCommand},
        {"PASS", &Request::passHandle},
        {"PWD", &Request::getpwdHandle},
        {"CWD", &Request::getcwdHandle},
        {"CDUP", &Request::getcdupHandle},
        {"PASV", &Request::pasvHandle},
        {"PORT", &Request::portHandle},
        {"LPRT", &Request::unvalidCommand},
        {"LIST", &Request::listHandle},
        {"RETR", &Request::retrHandle},
        {"QUIT", &Request::quitHandle}};

    std::vector<std::string> commands = {
        "USER",
        "PASS",
        "PWD",
        "CDUP",
        "CWD",
        "PORT",
        "PASV",
        "LIST",
        "LPRT",
        "SYST",
        "RETR",
        "QUIT"};

    // Server reply and status
    std::map<int, std::string> server_reply{
        {150, "150 - File status okay; about to open data connection."},
        {200, "200 - Command okay."},
        {220, "220 - Service ready for new user."},
        {221, "221 - Service closing."},
        {226, "226 - send ok"},
        {227, "227 - Entering passive mode"},
        {230, "230 - User logged in successfully."},
        {250, "250 - Requested file action okay, completed."},
        {331, "331 - User name okay, need a password."},
        {332, "332 - Need account for login."},
        {500, "500 - Syntax error, unkown command."},
        {501, "501 - Syntax error in parameters or arguments."},
        {502, "502 - Command not implemented."},
        {530, "530 - User password is wrong, didn't login."},
        {550, "550 - Requested action not taken. File unavailable."}};

public:
    Request(Ftp &);
    ~Request();

    void handle();
};

#endif
