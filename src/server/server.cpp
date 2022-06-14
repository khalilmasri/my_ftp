#include "server.hpp"
#include "logger.hpp"

int server_sock;

Server::Server(std::string portNumber, std::string filePath) {

    LOG_INFO("Initiating server with port %d", SERVER_PORT);

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
        LOG_ERR("socket err");
        exit(-1);
    }

    this->server_address.sin_port = portNumber;
    this->server_address.sin_family = AF_INET;
    this->server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    if((bind(server_sock, (struct sockaddr*)&this->server_address, sizeof(this->server_address))) < 0){
        LOG_ERR("Failed to bind");
        closeFD();
        exit(1);
    }
}

void Server::closeFD(){
    LOG_INFO("Disconnecting server...");
    close(server_sock);
    LOG_INFO("Disconnected!");
}

void Server::run(){

    if (listen(server_sock, MAX_THREAD_NUMBER) < 0)
    {
        LOG_ERR("Try again later");
        exit (-1);
    }
}

