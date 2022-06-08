#ifndef FTP_HPP
#define FTP_HPP

// LIBRARIES
#include <thread>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "server.hpp"

extern int server_sock;

#define MAX_TRANSMISSION_LENGTH 100000

class Ftp{
    private:
        int request_id;
        struct sockaddr_in request_addr;
        char buff[MAX_TRANSMISSION_LENGTH];
        socklen_t addr_size = sizeof(request_addr);

        void listen_request();
        void handle_request();

    public:
        Ftp();
};

#endif
