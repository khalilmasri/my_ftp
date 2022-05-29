#ifndef CLIENT_HPP
#define CLIENT_HPP

// LIBRARIES
#include <thread>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
//#include <sys/types.h>
#include  <arpa/inet.h>
#include <unistd.h>

#include "server.hpp"

extern int server_sock;

#define MAX_TRANSMISSION_LENGTH 100000

class Client{
    private:
        int client_ID;
        struct sockaddr_in client_addr;
        char buff[MAX_TRANSMISSION_LENGTH];
        socklen_t addr_size = sizeof(client_addr);

        void listen_request();
        void handle_request();

    public:
        Client();
};

#endif
