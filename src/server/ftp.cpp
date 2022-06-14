#include "ftp.hpp"
#include "logger.hpp"

Ftp::Ftp(){

    listen_request();

    getRequest();

    handle_request();
    close(this->request_id);
}

void Ftp::listen_request() {

    if ((this->request_id = accept(server_sock, (struct sockaddr*)&request_addr, &addr_size)) < 0)
    {
        LOG_ERR("accept Error");
        return;
    }

    std::cout << "I HEARD SOMRTHING\n";
}

void Ftp::getRequest() {

    while(current_state != 221){

        if((recv(request_id, buff, MAX_TRANSMISSION_LENGTH, 0)) == -1){
            LOG_ERR("%s", server_reply.at(500).c_str());
            continue;
        }

        parseCommand();
        handleCommand();
    }

}

void Ftp::parseCommand(){

    std::string command = buff;
    size_t pos = 0;

    while((pos = command.find(" ")) != std::string::npos){
        input.push_back(command.substr(0, pos));
        command.erase(0, pos + 1);
    }
    input.push_back(command.substr(0, pos));
    command.erase(0, pos + 1);
}

void Ftp::handleCommand(){

    for( auto it : commands){
        if(it == input.at(0)){
            (*this.*dispatch_table.at(it))();
            return;
        }
    } 
    
    LOG_ERR("%s", server_reply.at(500).c_str());
}

void Ftp::user_handle(){
    LOG_DEBUG("USER HANDLE");
}

void Ftp::pass_handle(){
    LOG_DEBUG("PASS HANDLE");
}

void Ftp::pasv_handle(){
    LOG_DEBUG("PASV HANDLE");
}

void Ftp::quit_handle(){
    LOG_DEBUG("QUIT");
    this->current_state = 221;
}

void Ftp::handle_request(){

    std::ifstream file (buff, std::ifstream::in);

    char message[MAX_TRANSMISSION_LENGTH];

    file.read(message, MAX_TRANSMISSION_LENGTH);

    send(request_id, message, MAX_TRANSMISSION_LENGTH, 0);

    file.close();
}


