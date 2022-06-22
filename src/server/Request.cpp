#include "request.hpp"
#include "logger.hpp"

Request::~Request(){};

void Request::handle(){

        std::cout << "Here\n";

    sendMsg(220);
    int ret = 0;

    while(current_state != 221){


        memset(&buff, 0, sizeof(buff));//clear the buffer
        if((ret = recv(getRequestID(), (char*)&buff, sizeof(buff), 0)) < 0){
            sendMsg(500);
            return;
        }

        buff[ret-1] = '\0';
        LOG_DEBUG("Got a commands %s", buff);

        parseCommand();
        handleCommand();    
 
        if(getAuth() == true){ 
            // handle_request();
            LOG_DEBUG("Authorized");
        }
    } 
}

void Request::parseCommand(){

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

void Request::handleCommand(){

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

void Request::userHandle(){

    if(input.size() > 2){
        sendMsg(500);
        return;
    }

    if (*input.begin() == "anonymous"){
        sendMsg(331);
        setUser(*input.begin());
    } else if(*input.begin() == "admin"){
        sendMsg(331);
        setUser(*input.begin());
    }else {
        sendMsg(501);
    }
}

void Request::passHandle(){

    if(input.size() > 2){
        sendMsg(530);
        return;
    }

    if(getUser() == "anonymous" && *input.begin() == ""){
        sendMsg(230);
        setPass(*input.begin());
        setAuth(true);
    }else if( getUser() == "admin" && *input.begin() == "admin"){
        sendMsg(230);
        setPass(*input.begin());
        setAuth(true);
    }else{
        sendMsg(530);
    }
}

void Request::pasvHandle(){


    //get server ip from server class
    Server* server = Server::getInstance();
    std::string server_ip = server->getServerIP();
    std::replace(server_ip.begin(), server_ip.end(), '.', ',');
    server_ip += ',';

    //create random number > 0 < 105 
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(20000, 30000); // define the range
    int p1 = distr(gen);
    int p2 = p1;
    p1 = p2 / 256;
    p2 = p2 % 256;

    LOG_DEBUG("P1: %d P2: %d", p1, p2);
    std::string PASV = server_ip + std::to_string(p1) + "," + std::to_string(p2);
    sendMsg(227, PASV);

    int dataPort = p1 * 256 + p2;
    sendMsg(220, std::to_string(dataPort));

    

}

void Request::listHandle(){
    
    if((dir = opendir(getFilePath().c_str())) == NULL){
        LOG_ERR("Couldn't find directory");
    }

    while((current_dir = readdir(dir)) != NULL){
        std::string name = current_dir->d_name;
        if(name != "." && name != ".."){
            file_list.push_back(name);
        }
    }
    
    for (auto it : file_list){
        sendMsg(150, it);
    }
}

void Request::unvalidCommand(){
    sendMsg(502);
}

void Request::quitHandle(){
    LOG_DEBUG("QUIT");
    sendMsg(221);
}

void Request::sendMsg(const int status){

    current_state = status;
    LOG_DEBUG("%s", server_reply.at(current_state).c_str());

    char msg[1500];

    memset(&msg, 0, sizeof(msg)); //clear the buffer
    strcpy(msg, server_reply.at(current_state).c_str());
    strcat(msg, "\r\n");

    send(getRequestID(), (char*)msg, strlen(msg),0);
}

void Request::sendMsg(const int status, std::string address){

    current_state = status;
    LOG_DEBUG("%s", server_reply.at(current_state).c_str());

    char msg[1500];

    memset(&msg, 0, sizeof(msg)); //clear the buffer
    strcpy(msg, server_reply.at(current_state).c_str());
    strcat(msg, " (");
    strcat(msg, address.c_str());
    strcat(msg, ")");
    strcat(msg, "\r\n");

    send(getRequestID(), (char*)msg, strlen(msg),0);
}

void Request::handle_request(){

    std::ifstream file (buff, std::ifstream::in);

    char message[MAX_TRANSMISSION_LENGTH];

    file.read(message, MAX_TRANSMISSION_LENGTH);

    send(getRequestID(), message, MAX_TRANSMISSION_LENGTH, 0);

    file.close();
}