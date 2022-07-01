#include "server.hpp"
#include "logger.hpp"

Server Server::server_instance;

Server::Server(){
    server_port = 0;
    server_sock = 0;
    file_path = "";
    server_ip = "127.0.0.1";
}

Server& Server::operator = (const Server& rhs){
    (void)rhs;
    return *this;
}

Server::~Server(){
}

void Server::Start() {

    LOG_INFO("Initiating server with port [%d] and file path [%s]", this->server_port, this->file_path.c_str());
    createServerSock();
    initListen();
}

void Server::createServerSock(){

    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        LOG_CRIT("socket error");
        exit(1);
    }

    
    int optval = 1;

    this->server_address.sin_port = htons(server_port);
    this->server_address.sin_family = AF_INET;
    this->server_address.sin_addr.s_addr = inet_addr(server_ip.c_str());


    LOG_INFO("Server connected to socket => %d", server_sock);
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

void Server::closeData(int data_socket) {
    LOG_INFO("Disconnecting Data...");
    shutdown(data_socket, SHUT_RDWR);
    close(data_socket);
}

void Server::setServerPort(const std::string port){
    this->server_port = std::stoi(port);
}

void Server::setFilePath(std::string file_path){

    std::ifstream file(file_path.c_str());

    if( false == (file.good())){
        LOG_CRIT("File path provided is not available");
        exit(1);
    } 

    this->file_path = file_path;
}

std::string Server::getServerIP(){
    return server_ip;
}

int Server::getServerPort(){
    return this->server_port;
}

int Server::getServerSocket(){
    return this->server_sock;
}

std::string Server::getFilePath(){
    return this->file_path;
}

void Server::initListen(){

    if (listen(server_sock, MAX_THREAD_NUMBER) < 0)
    {
        LOG_CRIT("Couldn't listen, Try again later");
        exit(1);
    }
}

