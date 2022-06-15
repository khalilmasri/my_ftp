#include "server.hpp"
#include "logger.hpp"

int server_sock;

Server::Server(const std::string port, const std::string filePath) {

    setServerPort(port);
    setFilePath(filePath);

    LOG_INFO("Initiating server with port [%d] and file path [%s]", this->server_port, this->file_path.c_str());

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
        exit(1);
    }

    int optval = 1;

    this->server_address.sin_port = server_port;
    this->server_address.sin_family = AF_INET;
    this->server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    
    if(setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR,&optval,sizeof(int)) < 0){
        closeServer();
        LOG_CRIT("Failed to set socket options!");
        exit(1);
    }

    if((bind(server_sock, (struct sockaddr*)&this->server_address, sizeof(this->server_address))) < 0){
        closeServer();
        LOG_CRIT("Failed to bind");
        exit(1);
    }
}

void Server::closeServer(){
    LOG_INFO("Disconnecting server...");
    shutdown(server_sock, SHUT_RDWR);
    close(server_sock);
    sleep(3);
    LOG_INFO("Disconnected!");
}

void Server::setServerPort(const std::string port){
    this->server_port = std::stoi(port);
}

void Server::setFilePath(std::string file_path){
    this->file_path = file_path;
}

void Server::run(){

    if (listen(server_sock, MAX_THREAD_NUMBER) < 0)
    {
        LOG_CRIT("Couldn't listen, Try again later");
        exit(1);
    }
}

