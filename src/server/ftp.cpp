#include "ftp.hpp"
#include "logger.hpp"


//static passive_mode
static bool passive_mode = false;

Ftp::Ftp(){

    Server* server = Server::getInstance();
    server_sock = server->getServerSocket();
    port = server->getServerPort();
    filePath = server->getFilePath();

    LOG_DEBUG("Socket => %d port => %d Path => %s", server_sock, port, filePath.c_str());
}

bool Ftp::listen_request() {

    if ((this->request_id = accept(server_sock, (struct sockaddr*)&request_addr, &addr_size)) < 0)
    {
        if(!detach){
            LOG_ERR("accept Error");
        }
        return false;
    }

    return true;
}

bool Ftp::listen_data(Server& data_server) {

    if (this->data_id = accept(data_server.getServerSocket(), (struct sockaddr*)&data_addr, &data_addr_size) < 0)
    {
        if(!detach){
            LOG_ERR("accept Error");
        }
        return false;
    }
    return true;
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

bool Ftp::getPASV() const{
    return this->pasv_mode;
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

void Ftp::setPASV(const bool pasv_mode){
    this->pasv_mode = pasv_mode;
}
std::string Ftp::getUser() const{
    return this->user;
}

std::string Ftp::getPass() const{
    return this->pass;
}

int Ftp::getDataID() const{
    return this->data_id;
}
