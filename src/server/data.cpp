#include "logger.hpp"
#include "data.hpp"

Data::Data(){}

Data::~Data(){}

int Data::connectData(int data_port, std::string data_ip){

    this->data_port = data_port;
    this->data_ip = data_ip;

    struct sockaddr_in data_address;

    if ((data_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        LOG_CRIT("socket error");
    }

    data_address.sin_port = htons(this->data_port);
    data_address.sin_family = AF_INET;
    data_address.sin_addr.s_addr = inet_addr(this->data_ip.c_str());

    return connect(data_socket,(sockaddr *)&data_address, sizeof(data_address));
}

bool Data::pasvHandle(int data_port, std::string filepath){

    this->data_port = data_port;
    this->filepath = filepath;

    passive_mode = true;

    this->data_server.setServerPort(std::to_string(data_port));
    this->data_server.setFilePath(filepath);
    this->data_server.Start();

    if (0 > server_sock){
        return false;
    }

    return true;
}

bool Data::acceptConnection() {

    struct sockaddr_in data_addr;
    socklen_t data_addr_size = sizeof(data_addr);

    this->data_socket = accept(this->data_server.getServerSocket(),
                              (struct sockaddr*)&data_addr, &data_addr_size);
    if (this->data_socket < 0)
    {
        if(!detach){
            LOG_ERR("accept Error");
        }
        return false;
    }

    return true;

}
bool Data::listHandle(std::string origin_path, std::string file_path) {


    std::string filename = origin_path + "/.tmp" + "/list" + ".txt";
    std::string command = "ls -l " + file_path + " > " + filename;

    std::system(command.c_str());

    std::ostringstream buff;
    std::ifstream outfile(filename.c_str());
    buff << outfile.rdbuf();
    std::string data = buff.str();

    std::cout << data << std::endl;

    sendData(data);

    command = "rm -f " + filename;
    std::system(command.c_str());

    if( true == passive_mode ){
        this->data_server.closeData(server_sock);
    }

    return true;
}

bool Data::retrHandle(std::string filepath)
{
    //read contents of file into buffer and then send it to client
    std::ifstream file(filepath);
    std::string line;
    std::string buffer;
    while (getline(file, line))
    {
        buffer += line;
        buffer += "\n";
    }

    file.close();

    sendData(buffer);

    return true;
}

void Data::sendData(std::string data)
{

    char msg[1500];

    memset(&msg, 0, sizeof(msg)); // clear the buffer
    strcat(msg, data.c_str());
    strcat(msg, "\r\n");

    send(this->data_socket, (char *)msg, strlen(msg), 0);
    close(this->data_socket);
}
