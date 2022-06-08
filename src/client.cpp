#include "../include/client.hpp"

Client::Client(){
    listen_request();
}

void Client::listen_request() {

    int result;
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(server_sock, &rfds);
    struct timeval tv;

    tv.tv_sec = (long)TIMEOUT;
    tv.tv_usec = 0;

    if ((this->client_ID = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size)) < 0)
    {
        std::cout << "accept Error\n";
        return;
    }
    
    handle_request();
}

void Client::handle_request() {
    std::cout << "HANDLING\n";
    close(this->client_ID);
}


