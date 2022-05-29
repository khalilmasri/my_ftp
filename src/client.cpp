#include "../include/client.hpp"

Client::Client(){
    listen_request();
    handle_request();
}

void Client::listen_request() {
    if ((this->client_ID = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size)) < 0)
    {
        std::cout << "accept Error\n";
        exit(-1);
    }
}

void Client::handle_request() {
    std::cout << "HANDLING\n";
}


