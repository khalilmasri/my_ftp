#ifndef CLIENT_HPP
#define CLIENT_HPP

// LIBRARIES
#include <thread>
#include <iostream>
#include <sys/socket.h>

class Client{

    private:
        int client_ID;
        bool available_c;

    public:
        Client(){
            std::cout << "here\n";
        }

        void create_client(int serverFD) {
            this->client_ID = -1;
            this->available_c = true;
        }
        
        bool get_available() const{
            return this->available_c;
        }
};

#endif
