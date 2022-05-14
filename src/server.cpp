#include "../include/server.hpp"

Server::Server(){
    create_server_sock();
    create_thread_pool();
}

/*
    thread made at beginning 
*/

Server::~Server(){
   
};

Server& Server::operator = (const Server& rhs){
    (void)rhs;
    return *this;
}

int Server::handleClient()  {
    int bytes;
    char buff[MAX_TRANSMISSION_LENGTH];
    int request_accept;
    struct sockaddr_in client_addr;
    socklen_t addr_size = sizeof(client_addr);
    int count = 0;
    while (1)
    {
        if ((request_accept = accept(this->server_sock, (struct sockaddr*)&client_addr, &addr_size)) < 0)
        {
            std::cout << "accept Error\n";
            exit(-1);
        }
        std::cout << "Client Addr = " << client_addr.sin_addr.s_addr << std::endl;
   
        bytes = read(request_accept, &buff, MAX_TRANSMISSION_LENGTH);
    //    std::cout << "Message-> " << buff << std::endl;
        count++;
        std::cout << count << std::endl;
    }
}

void Server::create_server_sock(){
    if ((this->server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "socket err\n";
        exit(-1);
    }

    this->server_address.sin_port = SERVER_PORT;
    this->server_address.sin_family = AF_INET;
    this->server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    if((bind(this->server_sock, (struct sockaddr*)&this->server_address, sizeof(this->server_address))) == -1){
        std::cout << "Failed to bind\n";
        exit(1);
    }
}

bool Server::create_thread_pool(void)
{
    for (int i = 0; i < POOL_SIZE; i++)
    {
        this->clients.push_back(Client(i));
    }
    this->client_number = POOL_SIZE;
}

void Server::run(){

    if (listen(this->server_sock, 5) < 0)
    {
        std::cout << "listen failed\n";
        exit (-1);
    }
    handleClient();
}

