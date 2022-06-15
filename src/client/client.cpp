#include "client.hpp"
#include "logger.hpp"

#include <unistd.h>

Client::Client(){
    createClient();
    sendRequest();
    /* receiveFile(); */
    readCommand();
    close(client_socket);
}

Client::~Client(){

}

void Client::createClient() {


    char *server_ip = "127.0.0.1";
    int port = 21;
    sockaddr_in sendSockAddr;
    struct hostent* host = gethostbyname(server_ip); 
    bzero((char*)&sendSockAddr, sizeof(sendSockAddr)); 
    sendSockAddr.sin_family = AF_INET; 
    sendSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    sendSockAddr.sin_port = port; 

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return;

    int status = connect(client_socket,
                         (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
    if(status < 0)
    {
        std::cout<<"Error connecting to socket!"<< std::endl;
    }

}

void Client::sendRequest(){
    
      char msg[1500];

    while(1)
    {
        std::cout << ">";
        std::string data;
        getline(std::cin, data);
        memset(&msg, 0, sizeof(msg));//clear the buffer
        strcpy(msg, data.c_str());
        if(data == "exit")
        {
            send(client_socket, (char*)&msg, strlen(msg), 0);
            break;
        }
        send(client_socket, (char*)&msg, strlen(msg), 0);
        std::cout << "Awaiting server response..." << std::endl;
        memset(&msg, 0, sizeof(msg));//clear the buffer
        recv(client_socket, (char*)&msg, sizeof(msg), 0);
        if(!strcmp(msg, "exit"))
        {
            std::cout << "Server has quit the session" << std::endl;
            break;
        }
        std::cout << "Server: " << msg << std::endl;
    }


        // std::ofstream file (arg, std::ofstream::out);

        // file.write(buffer, 10000);

        // file.close();
}

void Client::readCommand(){
    char buffer[MAX_CMD_SIZE];

    //read command from terminal
    std::cin.getline(buffer, MAX_CMD_SIZE);
    std::cout << "Command -> " << buffer << std::endl;
    
}


int main(){

    Client client;

}
