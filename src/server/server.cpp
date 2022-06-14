#include "server.hpp"
#include "logger.hpp"

int server_sock;

Server::Server() {

    LOG_INFO("Initiating server with port %d", SERVER_PORT);

    createServerSock();
    run();
}

Server& Server::operator = (const Server& rhs){
    (void)rhs;
    return *this;
}

Server::~Server(){};


void Server::createServerSock(){
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        LOG_CRIT("socket error");
    }

    this->server_address.sin_port = SERVER_PORT;
    this->server_address.sin_family = AF_INET;
    this->server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    if((bind(server_sock, (struct sockaddr*)&this->server_address, sizeof(this->server_address))) < 0){
        closeServer();
        LOG_CRIT("Failed to bind");
    }
}

void Server::closeServer(){
    LOG_INFO("Disconnecting server...");
    close(server_sock);
    LOG_INFO("Disconnected!");
}

void Server::run(){

    if (listen(server_sock, MAX_THREAD_NUMBER) < 0)
    {
        LOG_CRIT("Try again later");
    }
}

