#ifndef FTP_SERVER
#define FTP_SERVER


// LIBRARIES
#include <list>
#include <netinet/in.h>
#include <sys/socket.h>
//#include <sys/types.h>
#include  <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <iostream>

#define POOL_SIZE 5
#define SERVER_PORT 21
#define MAX_TRANSMISSION_LENGTH 100000
#define MAX_THREAD_NUMBER 256

#define ERROR_SOCKET -3

extern int server_sock;

class Server{

    private: 
        struct sockaddr_in server_address;
        void createServerSock();
        void setFilePath(std::string);
        void setServerPort(std::string);

        int server_port;
        std::string file_path;
        
    public:
        Server(std::string, std::string);
        Server& operator = (const Server&);
        ~Server();

        // METHODS
        void run();
        void closeServer();
        int  getServerSockID();
};


#endif
