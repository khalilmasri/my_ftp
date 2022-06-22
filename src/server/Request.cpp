#include "request.hpp"
#include "logger.hpp"

Request::Request(Ftp &ftp_com){
    this->ftp_com = ftp_com;
}

Request::~Request(){};

void Request::handle(){

    sendMsg(220);
    int ret = 0;

    while(current_state != 221){

        memset(&buff, 0, sizeof(buff));//clear the buffer
        if((ret = recv(ftp_com.getRequestID(), (char*)&buff, sizeof(buff), 0)) < 0){
            sendMsg(500);
            return;
        }

        buff[ret-1] = '\0';
        LOG_DEBUG("Got a commands %s", buff);

        parseCommand();
        handleCommand();    
 
        if(ftp_com.getAuth() == true){ 
            // handle_request();
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
        ftp_com.setUser(*input.begin());
    } else if(*input.begin() == "admin"){
        sendMsg(331);
        ftp_com.setUser(*input.begin());
    }else {
        sendMsg(501);
    }
}

void Request::passHandle(){

    if(input.size() > 2){
        sendMsg(530);
        return;
    }

    if(ftp_com.getUser() == "anonymous"){
        sendMsg(230);
        ftp_com.setPass(*input.begin());
        ftp_com.setAuth(true);
    }else if( ftp_com.getUser() == "admin" && *input.begin() == "admin"){
        sendMsg(230);
        ftp_com.setPass(*input.begin());
        ftp_com.setAuth(true);
    }else{
        sendMsg(530);
        return;
    }
    
    LOG_DEBUG("Authorized");
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

void Request::getpwdHandle()
{
    char pwd[MAX_PATH];
    getcwd(pwd, MAX_PATH);
    sendMsg(250, pwd);
}

void Request::getcwdHandle()
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

void Request::getcdupHandle(){
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

void Request::listHandle(){
    
    if((dir = opendir(ftp_com.getFilePath().c_str())) == NULL){
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
    sendData(150, list);
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

    send(ftp_com.getRequestID(), (char*)msg, strlen(msg),0);
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

    send(ftp_com.getRequestID(), (char*)msg, strlen(msg),0);
}

void Request::sendData(const int status, std::string data){

    current_state = status;
    std::cout << data << std::endl;
    LOG_DEBUG("%s", server_reply.at(current_state).c_str());

    char msg[1500];

    memset(&msg, 0, sizeof(msg)); //clear the buffer
    strcpy(msg, server_reply.at(current_state).c_str());
    strcat(msg, " (");
    strcat(msg, data.c_str());
    strcat(msg, ")");
    strcat(msg, "\r\n");

    send(this->data_socket, (char*)msg, strlen(msg),0);

}

void Request::portHandle(){

    std::string deli = ",";
    int pos = 0;
    int index = 0;
    this->data_ip = "";
    std::string digit = "";
    std::string full_ip = *input.begin();

    std::cout << full_ip << std::endl;
    while((pos = full_ip.find(deli)) != std::string::npos){
        digit = full_ip.substr(0, pos);
        full_ip.erase(0, pos + deli.length());

        if(index < 3){
            this->data_ip += digit;
            this->data_ip += deli;
        }else if(index == 3){
            this->data_ip += digit;
        }else{
            if(index == 4){
                this->data_port = std::stoi(digit) * 256;
            }
        }

        index++;
    }

    digit = full_ip.substr(0, pos);
    this->data_port += std::stoi(digit);

    LOG_DEBUG("Data IP => %s", this->data_ip.c_str());
    LOG_DEBUG("Data Port => %d", this->data_port);

    handle_request();

    sendMsg(200);
}

void Request::handle_request(){

    struct sockaddr_in data_address;

    if ((data_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        LOG_CRIT("socket error");
        exit(1);
    }

    int optval = 1;

    data_address.sin_port = htons(this->data_port);
    data_address.sin_family = AF_INET;
    data_address.sin_addr.s_addr = inet_addr(this->data_ip.c_str());

    int status = connect(data_socket,
                         (sockaddr*) &data_address, sizeof(data_address));

}
