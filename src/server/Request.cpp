#include "request.hpp"
#include "logger.hpp"

Request::Request(Ftp &ftp_com)
{
    this->ftp_com = ftp_com;
}

Request::~Request(){};

void Request::handle()
{

    sendMsg(220);
    setOriginPath();

    this->current_path = this->origin_path;

    int ret = 0;

    while (current_state != 221)
    {

        memset(&buff, 0, sizeof(buff)); // clear the buffer
        if ((ret = recv(ftp_com.getRequestID(), (char *)&buff, sizeof(buff), 0)) < 0)
        {
            sendMsg(500, "handle() error!");
            return;            
        }

        buff[ret - 1] = '\0';
        LOG_DEBUG("Got command: %s", buff);

        parseCommand();
        handleCommand();
    }
}

void Request::parseCommand()
{

    std::string command = buff;
    std::string cmd;

    std::istringstream iss;
    iss.str(command);

    int index = 0, count = 0;
    while (buff[index])
    {
        if (buff[index] == ' ')
        {
            count++;
        }
        index++;
    }

    count++;
    for (int i = 0; i < count; i++)
    {
        iss >> cmd;
        input.push_back(cmd);
    }
}

void Request::handleCommand()
{

    for (auto it : commands)
    {
        if (it == *input.begin())
        {
            input.erase(input.begin());
            (*this.*dispatch_table.at(it))();
            input.clear();
            return;
        }
    }

    sendMsg(500, *input.begin());
    input.clear();
}

void Request::userHandle()
{

    if (input.size() > 2)
    {
        sendMsg(500, "USER");
        return;
    }

    if (*input.begin() == "Anonymous")
    {
        sendMsg(331);
    }
    else if (*input.begin() == "admin")
    {
        sendMsg(331);
    }
    else
    {
        sendMsg(501);
        return;
    }

    ftp_com.setUser(*input.begin());
}
void Request::setOriginPath()
{
    char origin[MAX_PATH];
    getcwd(origin, 200);
    origin_path = origin;
    origin_path_size = origin_path.length();
    LOG_INFO("Origin path => %s Origin path length => %d", origin_path.c_str(), origin_path_size);
}

std::string Request::getOriginPath()
{
    return origin_path;
}

void Request::passHandle()
{

    if (input.size() > 2)
  {
        sendMsg(530);
        return;
    }

    if (ftp_com.getUser() == "Anonymous")
    {
        sendMsg(230);
    }
    else if (ftp_com.getUser() == "admin" && *input.begin() == "admin")
    {
        sendMsg(230);
    }
    else
    {
        sendMsg(530);
        return;
    }

    ftp_com.setPass(*input.begin());
    ftp_com.setAuth(true);
    LOG_DEBUG("Authorized");
}

void Request::pasvHandle()
{

    std::string server_ip = this->ftp_com.getServerIP();
    std::replace(server_ip.begin(), server_ip.end(), '.', ',');
    server_ip += ',';
    passive_mode = true;

    std::random_device rd;                               // obtain a random number from hardware
    std::mt19937 gen(rd());                              // seed the generator
    std::uniform_int_distribution<> distr(32000, 60000); // define the range
    int p1 = distr(gen);
    int p2 = p1;
    p1 = p2 / 256;
    p2 = p2 % 256;

    int data_port = p1 * 256 + p2;
    std::string PASV = server_ip + std::to_string(p1) + "," + std::to_string(p2);

    bool status = data.pasvHandle(data_port, this->current_path);
     if( false == status ){
        sendMsg(500, "PASV");
        return;
    }

    sendMsg(227, PASV);

    status = data.acceptConnection();
     if( false == status ){
        sendMsg(500, "PASV");
        return;
    }

}

void Request::getpwdHandle()
{
    std::string pwd = "." + this->current_path.substr(this->origin_path_size);
    sendMsg(250, pwd);
}

void Request::getcwdHandle()
{
    if (input.size() > 2)
    {
        sendMsg(500, "CWD");
        return;
    }

    int res = 0;
    std::string cwd = *input.begin();

    if (cwd == "..")
    {
        getcdupHandle();
        return;
    }

    res = access(cwd.c_str(), F_OK);

    if( -1 == res ){
        sendMsg(501, "Directory doesn't exist");
        return;
    }

    cwd = this->current_path + "/" + (*input.begin());
    res = access(cwd.c_str(), F_OK);

    if ( -1 == res)
    {
        sendMsg(501, "Directory doesn't exist");
        return;
    }

    this->current_path = cwd;

    cwd = "./" + this->current_path.substr(this->origin_path_size+1);
    sendMsg(250, cwd);
}

void Request::getcdupHandle()
{

    std::string cwd = this->current_path;

    int i = cwd.length();
    
    while (cwd[i] != '/')
    {
        i--;
    }

    cwd.resize(i);

    if( i < this->origin_path_size){
        sendMsg(501, "Parent directory is not accesssable");
        return;
    }

    this->current_path = cwd;
    cwd = "." + this->current_path.substr(this->origin_path_size);
    sendMsg(250, cwd);
}

void Request::listHandle()
{

    if (ftp_com.getAuth() == false)
    {
        sendMsg(530);
        return;
    }

    std::string path;
    if( 1 == input.size()){
        path = this->current_path + "/" + *input.begin();
    }else if ( 0 == input.size()){
        path = this->current_path;
    }else{
        sendMsg(500, "LIST FAILED");
    }

    sendMsg(150);

    bool status = data.listHandle(origin_path, path);

    if( true == status){
        sendMsg(226, "Listing directort/file");
    }else{
        sendMsg(500, "LIST FAILED");
    }

}

void Request::unvalidCommand()
{
    sendMsg(502);
}

void Request::quitHandle()
{
    LOG_DEBUG("QUIT");
    sendMsg(221);
}

void Request::sendMsg(const int status)
{

    current_state = status;
    LOG_DEBUG("%s", server_reply.at(current_state).c_str());

    char msg[1500];

    memset(&msg, 0, sizeof(msg)); // clear the buffer
    strcpy(msg, server_reply.at(current_state).c_str());
    strcat(msg, "\r\n");

    send(ftp_com.getRequestID(), (char *)msg, strlen(msg), 0);
}

void Request::sendMsg(const int status, std::string data)
{

    current_state = status;
    LOG_DEBUG("%s (%s)", server_reply.at(current_state).c_str() , data.c_str());

    char msg[1500];

    memset(&msg, 0, sizeof(msg)); // clear the buffer
    strcpy(msg, server_reply.at(current_state).c_str());
    strcat(msg, " (");
    strcat(msg, data.c_str());
    strcat(msg, ")");
    strcat(msg, "\r\n");

    send(ftp_com.getRequestID(), (char *)msg, strlen(msg), 0);
}



void Request::portHandle()
{

    if (ftp_com.getAuth() == false)
    {
        sendMsg(530);
    }

    std::string deli = ",";
    int pos = 0;
    int index = 0;
    std::string data_ip = "";
    int data_port;
    std::string digit = "";
    std::string full_ip = *input.begin();

    while ((pos = full_ip.find(deli)) != std::string::npos)
    {
        digit = full_ip.substr(0, pos);
        full_ip.erase(0, pos + deli.length());

        if (index < 3)
        {
            data_ip += digit;
            data_ip += deli;
        }
        else if (index == 3)
        {
            data_ip += digit;
        }
        else
        {
            if (index == 4)
            {
                data_port = std::stoi(digit) * 256;
            }
        }

        index++;
    }

    digit = full_ip.substr(0, pos);
    data_port += std::stoi(digit);
    std::replace(data_ip.begin(), data_ip.end(), ',', '.');

    LOG_DEBUG("Data IP => %s", data_ip.c_str());
    LOG_DEBUG("Data Port => %d", data_port);

    sendMsg(200);

    int status = data.connectData(data_port, data_ip);

    if (status < 0)
    {
        LOG_ERR("Connection failed");
        sendMsg(500, "PORT");
    }
}

void Request::retrHandle()
{

    if (ftp_com.getAuth() == false)
    {
        sendMsg(530);
        return;
    }

    if (input.size() > 2)
    {
        sendMsg(530);
        return;
    }

    std::string filename = *input.begin();
    std::string filepath = this->current_path + "/" + filename;
    LOG_DEBUG("Filepath => %s", filepath.c_str());

    if (access(filepath.c_str(), F_OK) == -1)
    {
        sendMsg(550);
        return;
    }

    sendMsg(150);
    
    bool status = data.retrHandle(filepath);

    if( status == true){
        sendMsg(226);
    }else{
        sendMsg(500, "RETR FAILED");
    }
}
