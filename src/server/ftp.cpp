#include "ftp.hpp"
#include "logger.hpp"

Ftp::Ftp(){

    Server* server = Server::getInstance();
    server_sock = server->getServerSocket();
    port = server->getServerPort();
    filePath = server->getFilePath();

    LOG_DEBUG("Socket => %d port => %d Path => %s", server_sock, port, filePath.c_str());
    listen_request();

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

    getRequest();
    // handle_request();
}

void Ftp::getRequest() {

    sendMsg(220);
    int ret = 0;

    while(current_state != 221){


        memset(&buff, 0, sizeof(buff));//clear the buffer
        if((ret = recv(request_id, (char*)&buff, sizeof(buff), 0)) < 0){
            sendMsg(500);
            return;
        }

        buff[ret-1] = '\0';
        LOG_DEBUG("Got a commands %s", buff);

        parseCommand();
        handleCommand();    
 
        if(authorized == true){ 
            // handle_request();
            LOG_DEBUG("Authorized");
        }
    } 

}

void Ftp::parseCommand(){

    std::string command = buff;
    std::string cmd;

    std::istringstream iss;
    iss.str(command);

    int index = 0, count = 0;
    while(buff[index]){
        if(buff[index] == ' '){
            count++;
        }
        index++;
    }
    
    count++;
    for(int i = 0;i < count; i++){
        iss >> cmd;
        input.push_back(cmd);
    }
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

void Ftp::userHandle(){

    if(input.size() > 2){
        sendMsg(500);
        return;
    }

    if (*input.begin() == "anonymous"){
        sendMsg(331);
        user = *input.begin();
    } else if(*input.begin() == "admin"){
        sendMsg(331);
        user = *input.begin();
    }else {
        sendMsg(501);
    }
}

void Ftp::passHandle(){

    if(input.size() > 2){
        sendMsg(530);
        return;
    }

    if(user == "anonymous" && *input.begin() == ""){
        sendMsg(230);
        authorized = true;
    }else if( user == "admin" && *input.begin() == "admin"){
        sendMsg(230);
        authorized = true;
    }else{
        sendMsg(530);
    }
}

void Ftp::pasvHandle(){
    
    //create random number > 0 < 105 
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, 105); // define the range
    int p1 = distr(gen) + 12; // generate random number between 0-39 + 78
    int p2 = distr(gen); // generate random number between 0-105

    //print numbers
    LOG_DEBUG("P1: %d P2: %d", p1, p2);

}

void Ftp::listHandle(){
    LOG_DEBUG("LIST HANDLE");
}

void Ftp::unvalidCommand(){
    sendMsg(502);
}

void Ftp::quitHandle(){
    LOG_DEBUG("QUIT");
    sendMsg(221);
}

void Ftp::sendMsg(const int status){

    current_state = status;
    LOG_DEBUG("%s", server_reply.at(current_state).c_str());

    char msg[1500];

    memset(&msg, 0, sizeof(msg)); //clear the buffer
    strcpy(msg, server_reply.at(current_state).c_str());
    strcat(msg, "\r\n");

    send(request_id, (char*)msg, sizeof(msg),0);
}

void Ftp::handle_request(){

    std::ifstream file (buff, std::ifstream::in);

    char message[MAX_TRANSMISSION_LENGTH];

    file.read(message, MAX_TRANSMISSION_LENGTH);

    send(request_id, message, MAX_TRANSMISSION_LENGTH, 0);

    file.close();
}


