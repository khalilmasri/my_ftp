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

    if(user == "anonymous"){
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

void Ftp::getpwdHandle()
{
    char pwd[MAX_PATH];
    getcwd(pwd, MAX_PATH);
    sendMsg(250, pwd);
}

void Ftp::getcwdHandle()
{
    if (input.size() > 2)
    {
        sendMsg(500);
        return;
    }

    std::string cwd = *input.begin();
    if (cwd == ".."){
        getcdupHandle();
        return;
    } else if (chdir(cwd.c_str()) == 0)
    {
        sendMsg(250, cwd);
    } else
        sendMsg(550);
}

void Ftp::getcdupHandle(){
    char cwd[200];
    getcwd(cwd, 100);
    int i = strlen(cwd);
    while(cwd[i] != '/'){
        i--;
    }
    cwd[i] = '\0';
    chdir(cwd);
    sendMsg(250, cwd);
}

void Ftp::listHandle(){
    
    if((dir = opendir(filePath.c_str())) == NULL){
        LOG_ERR("Couldn't find directory");
    }

    while((current_dir = readdir(dir)) != NULL){
        std::string name = current_dir->d_name;
        if(name != "." && name != ".."){
            file_list.push_back(name);
        }
    }
    
    std::string list = "\n";
    for (auto it : file_list){
        list = list + it + "\n";
    }
    sendMsg(150, list);
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

    send(request_id, (char*)msg, strlen(msg),0);
}

void Ftp::sendMsg(const int status, std::string address){

    current_state = status;
    LOG_DEBUG("%s", server_reply.at(current_state).c_str());

    char msg[1500];

    memset(&msg, 0, sizeof(msg)); //clear the buffer
    strcpy(msg, server_reply.at(current_state).c_str());
    strcat(msg, " (");
    strcat(msg, address.c_str());
    strcat(msg, ")");
    strcat(msg, "\r\n");

    send(request_id, (char*)msg, strlen(msg),0);
}

void Ftp::handle_request(){

    std::ifstream file (buff, std::ifstream::in);

    char message[MAX_TRANSMISSION_LENGTH];

    file.read(message, MAX_TRANSMISSION_LENGTH);

    send(request_id, message, MAX_TRANSMISSION_LENGTH, 0);

    file.close();
}


