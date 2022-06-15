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
        if(!detach){
             LOG_ERR("accept Error");
        }
        return;
    }

    getRequest();
    // handle_request();
}

using namespace std;

void Ftp::getRequest() {

    while(current_state != 221){

        memset(&buff, 0, sizeof(buff));//clear the buffer
        
        if((recv(request_id, (char*)&buff, sizeof(buff), 0)) < 0){
            sendMsg(500);
            return;
        }

        parseCommand();
        handleCommand();

        if(authorized == true){ 
            handle_request();
            LOG_DEBUG("Authorized");
        }
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
    sendMsg(500);
}

void Ftp::user_handle(){

    if(input.size() > 2){
        sendMsg(500);
        return;
    }

    if (*input.begin() == "Anonymous"){
        sendMsg(331);
        user = *input.begin();
    } else if(*input.begin() == "admin"){
        sendMsg(331);
        user = *input.begin();
    }else {
        sendMsg(332);
    }
}

void Ftp::pass_handle(){

    if(input.size() > 2){
        sendMsg(530);
        return;
    }

    if(user == "Anonymous" && *input.begin() == ""){
        sendMsg(230);
        authorized = true;
    }else if( user == "admin" && *input.begin() == "admin"){
        sendMsg(230);
        authorized = true;
    }else{
        sendMsg(530);
    }
}

void Ftp::pasv_handle(){
    LOG_DEBUG("PASV HANDLE");
}

void Ftp::list_handle(){
    LOG_DEBUG("LIST HANDLE");
}

void Ftp::quit_handle(){
    LOG_DEBUG("QUIT");
    sendMsg(221);
}

void Ftp::sendMsg(const int status){

    current_state = status;
    LOG_DEBUG("%s", server_reply.at(current_state).c_str());

    char msg[1500];

    memset(&msg, 0, sizeof(msg)); //clear the buffer
    strcpy(msg, server_reply.at(current_state).c_str());

    send(request_id, (char*)msg, sizeof(msg),0);
}

void Ftp::handle_request(){

    std::ifstream file (buff, std::ifstream::in);

    char message[MAX_TRANSMISSION_LENGTH];

    file.read(message, MAX_TRANSMISSION_LENGTH);

    send(request_id, message, MAX_TRANSMISSION_LENGTH, 0);

    file.close();
}


