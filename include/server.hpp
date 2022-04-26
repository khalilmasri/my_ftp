#ifndef FTP_SERVER
#define FTP_SERVER

// HEADERS
#include "client.hpp"

// LIBRARIES
#include <list>
#include <netinet/in.h>
#include <sys/socket.h>
//#include <sys/types.h>
#include  <arpa/inet.h>
#include <unistd.h>
#include <vector>
#define POOL_SIZE 5
#define SERVER_PORT 8080
#define MAX_TRANSMISSION_LENGTH 100


#define ERROR_SOCKET -3
class Server{

    private: 
        int server_sock;
        struct sockaddr_in server_address;
        std::thread clients[5];
        int client_number;
        std::vector<int> fd;
        bool newClient;

        bool getNewClient() const{
            return newClient;
        }

        int handleClient()  {
            int bytes;
            char *buff;
            while (1)
            {
                bytes = read(server_sock, &buff, MAX_TRANSMISSION_LENGTH);
            }
        }

        void create_server_sock(){
            if ((this->server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            {
                std::cout << "socket err\n";
                exit(-1);
            }
  
            this->server_address.sin_port = SERVER_PORT;
            this->server_address.sin_family = AF_INET;
            this->server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
            
            if((bind(this->server_sock, (struct sockaddr*)&this->server_address, sizeof(this->server_address))) == -1){
                std::cout << "Failed to bind\n";
                exit(1);
            }
        }

    public:
        Server(){
            create_server_sock();
          
            for(int i = 0; i < POOL_SIZE; i++){
                Client c;
            }
            client_number = 0;
            newClient = false;

        }
        /* Server(const Server&); */
        ~Server(){};
        Server& operator = (const Server&);

        // METHODS
        void run(){
            struct sockaddr_in client_addr;
            socklen_t addr_size = sizeof(client_addr);
            int request_accept;

            if (listen(this->server_sock, 5) < 0)
            {
                std::cout << "listen failed\n";
                exit (-1);
            }
            if ((request_accept = accept(this->server_sock, (struct sockaddr*)&client_addr, &addr_size)) < 0)
            {
                std::cout << "accept Error\n";
                exit(-1);
            }
            client_number++;
            fd.push_back(request_accept);
            newClient = true;
        };
};


#endif
