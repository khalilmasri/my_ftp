#include "ftp.hpp"
#include "logger.hpp"
#include "request.hpp"


Ftp::Ftp(){

    Server* server = Server::getInstance();
    server_sock = server->getServerSocket();
    port = server->getServerPort();
    filePath = server->getFilePath();

    LOG_DEBUG("Socket => %d port => %d Path => %s", server_sock, port, filePath.c_str());
    listen_request();

        LOG_DEBUG("Out here");

    close(this->request_id);
}

void Ftp::listen_request() {

    if ((this->request_id = accept(server_sock, (struct sockaddr*)&request_addr, &addr_size)) < 0)
    {
        if(!detach){
            LOG_ERR("accept Error");
        }
        return;
    }

    LOG_DEBUG("Creating request");
    Request request;
    request.handle();
    LOG_DEBUG("Out here");

}


int Ftp::getServerSock() const{
    return this->server_sock;
}

int Ftp::getServerPort() const{
    return this->port;
}

std::string Ftp::getFilePath() const{
    return this->filePath;
}

int Ftp::getDataPort() const{
    return this->dataPort;
}

int Ftp::getRequestID() const{
    return this->request_id;
}

bool Ftp::getAuth() const{
    return this->authorized;
}

void Ftp::setUser(const std::string user){
    this->user = user;
}

void Ftp::setPass(const std::string pass){
    this->pass = pass;
}

void Ftp::setAuth(const bool authorized){
    this->authorized = authorized;
}

std::string Ftp::getUser() const{
    return this->user;
}

std::string Ftp::getPass() const{
    return this->pass;
}
