#include "ftp.hpp"

Ftp::Ftp(){
    listen_request();
}

void Ftp::listen_request() {

    if ((this->request_id = accept(server_sock, (struct sockaddr*)&request_addr, &addr_size)) < 0)
    {
        std::cout << "accept Error\n";
        return;
    }
    
    handle_request();
}

void Ftp::handle_request() {
    std::cout << "HANDLING\n";
    close(this->request_id);
}


