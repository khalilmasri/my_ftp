#include "server.hpp"
#include "logger.hpp"

int server_sock;

Server::Server(std::string portNumber, std::string filePath) {
    DBG_PRINT_LOGGER("Initiating server...");
    //get port number and file path from server initiation
    getPortNumber(portNumber);
    getFiePath(filePath);

    std::cout << "Port number: " << portNumber << std::endl;
    std::cout << "File path: " << filePath << std::endl;

    create_server_sock();
    run();
}

Server::~Server(){};

Server& Server::operator = (const Server& rhs){
    (void)rhs;
    return *this;
}

void Server::getPortNumber(std::string portNumber){
    this->portNumber = std::stoi(portNumber);
}

void Server::getFiePath(std::string filePath){
    this->filePath = filePath;
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
    close(server_sock);
    DBG_PRINT_LOGGER("Disconnected!");
}

void Server::run(){

    if (listen(server_sock, MAX_THREAD_NUMBER) < 0)
    {
        std::cout << "Try again later\n";
        exit (-1);
    }
}

