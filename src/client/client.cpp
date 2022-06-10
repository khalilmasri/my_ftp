#include "client.hpp"


Client::Client(){
    createClient();
    sendRequest();
    /* receiveFile(); */
    close(client_socket);
}

Client::~Client(){

}

void Client::createClient() {

    struct sockaddr_in client;

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return;

    client.sin_family = AF_INET;
    client.sin_port = SERVER_PORT;
    client.sin_addr.s_addr = inet_addr("127.0.0.1");

    int connect_socket = connect(client_socket, (struct sockaddr *) &client, sizeof(client));
}

void Client::sendRequest(){
    
    char arg[] = "test.txt";
    send(client_socket, arg,9, 0 );
    
    char buffer[10000];
    recv(client_socket, buffer, 10000, 0);
    std::cout << "Message recieved -> " << buffer << std::endl;

    std::ofstream file (arg, std::ofstream::out);

    file.write(buffer, 10000);

    file.close();
}


int main(){

    Client client;

}