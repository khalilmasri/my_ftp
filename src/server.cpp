#include "server.hpp"
#include "logger.hpp"

int server_sock;

Server::Server(){
    DBG_PRINT_LOGGER("Initiating server...");
    create_server_sock();
    run();
}

Server::~Server(){};

Server& Server::operator = (const Server& rhs){
    (void)rhs;
    return *this;
}

void Server::create_server_sock(){
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "socket err\n";
        exit(-1);
    }

    this->server_address.sin_port = SERVER_PORT;
    this->server_address.sin_family = AF_INET;
    this->server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    if((bind(server_sock, (struct sockaddr*)&this->server_address, sizeof(this->server_address))) < 0){
        std::cout << "Failed to bind\n";
        closeFD();
        exit(1);
    }
}

void Server::closeFD(){
    DBG_PRINT_LOGGER("Disconnecting server...");
    DBG_PRINT_LOGGER("Disconnected!");
    close(server_sock);
}

void Server::run(){

    if (listen(server_sock, MAX_THREAD_NUMBER) < 0)
    {
        std::cout << "Try again later\n";
        exit (-1);
    }
}

