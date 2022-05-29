#ifndef CLIENT_HPP
#define CLIENT_HPP

// LIBRARIES
#include <thread>
#include <iostream>
#include <sys/socket.h>

#define MAX_TRANSMISSION_LENGTH 100000

class Client{

    private:
        // int client_ID;
        // bool available_c;
        // struct sockaddr_in client_addr;
        // char buff[MAX_TRANSMISSION_LENGTH];
        // socklen_t addr_size = sizeof(client_addr);
        // int server_fd;

    public:
        // Client(){
        //     this->client_ID = 0;
        //     this->available_c = true;
        // }

        // void listen_request(int serverFD) {
        //     if ((this->client_ID = accept(serverFD, (struct sockaddr*)&client_addr, &addr_size)) < 0)
        //     {
        //         std::cout << "accept Error\n";
        //         exit(-1);
        //     }
        //     this->available_c = false;
        // }
        
        bool get_available() const{
            // return this->available_c;
        }
};

#endif