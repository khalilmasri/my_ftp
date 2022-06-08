#include "ftp.hpp"

Ftp::Ftp(){
    listen_request();
    close(this->request_id);
}

void Ftp::listen_request() {

    if ((this->request_id = accept(server_sock, (struct sockaddr*)&request_addr, &addr_size)) < 0)
    {
        std::cout << "accept Error\n";
        return;
    }
    
    getRequest();
}

void Ftp::getRequest() {
    recv(request_id, buff, MAX_TRANSMISSION_LENGTH, 0);
    std::cout << "Message -> " << buff << std::endl;

    handle_request();
}

void Ftp::handle_request(){

    std::ifstream file (buff, std::ifstream::in);
    
    char message[MAX_TRANSMISSION_LENGTH];

    file.read(message, MAX_TRANSMISSION_LENGTH);

    send(request_id, message, MAX_TRANSMISSION_LENGTH, 0);
    
    file.close();
}

