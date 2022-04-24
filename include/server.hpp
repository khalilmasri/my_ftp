#ifndef FTP_SERVER
#define FTP_SERVER

// HEADERS
#include "client.hpp"

// LIBRARIES
#include <list>
#include <netinet/in.h>
#include <sys/socket.h>

#define POOL_SIZE 5
#define SERVER_PORT 8080

class Server{

    private: 
        int server_sock;
        struct sockaddr_in server_address;
        std::thread clients[5];

        void create_server_sock(){
            this->server_sock = socket(AF_INET, SOCK_STREAM, 0);

            if(server_sock < 0){
                std::cout << "server_sock" << std::endl;
                exit(1);
            }
        }
        void create_server_address(){
            this->server_address.sin_port = SERVER_PORT;
            this->server_address.sin_family = AF_INET;
            this->server_address.sin_addr.s_addr = INADDR_ANY;
        }

        void bind_server_address(){
            if((bind(this->server_sock, (struct sockaddr*)&this->server_address, sizeof(this->server_address))) == -1){
                std::cout << "Failed to bind\n";
                exit(1);
            }
        }

    public:
        Server(){
            create_server_sock();
            create_server_address();
            bind_server_address();

            for(int i = 0; i < POOL_SIZE; i++){
                Client c;
                clients[i] = std::thread(&Client::create_client, std::ref(c), this->server_sock);
            }
        }
        /* Server(const Server&); */
        ~Server(){};
        Server& operator = (const Server&);

        // METHODS
        void run(){
            listen(this->server_sock, 5);

            while(1){
                struct sockaddr_in client_addr;
                socklen_t addr_size = sizeof(client_addr);
                int request_accept = accept(this->server_sock, (struct sockaddr*)&client_addr, &addr_size);
                for(int i = 0; i < POOL_SIZE; i++){
                    clients[i].detach
                }
            }
        };
};


#endif
