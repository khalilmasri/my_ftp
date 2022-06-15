#include "ftp.hpp"
#include "logger.hpp"

Ftp::Ftp(){

    listen_request();

    LOG_DEBUG("Finished sending");
    close(this->request_id);
}

void Ftp::listen_request() {

    LOG_DEBUG("Accept from socket => %d", server_sock);
    if ((this->request_id = accept(server_sock, (struct sockaddr*)&request_addr, &addr_size)) < 0)
    {
        LOG_ERR("accept Error");
        return;
    }

    getRequest();
    // handle_request();
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

    input.push_back(command);
}

void Ftp::handleCommand(){

    for( auto it : commands){
        if(it == *input.begin()){
            input.erase(input.begin());
            (*this.*dispatch_table.at(it))();
            input.clear();
            return;
        }
    } 
    
    input.clear();
    current_state = 500;
    LOG_ERR("%s", server_reply.at(500).c_str());
}

void Ftp::user_handle(){
    LOG_DEBUG("USER HANDLE");
    if (*input.begin() == "admin"){
        LOG_DEBUG("LOGGED IN");
    }
}

void Ftp::pass_handle(){
    LOG_DEBUG("PASS HANDLE");
}

void Ftp::pasv_handle(){
    LOG_DEBUG("PASV HANDLE");
}

void Ftp::list_handle(){
    LOG_DEBUG("LIST HANDLE");
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


