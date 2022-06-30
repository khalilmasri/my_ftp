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
#include <fstream>

#define SERVER_PORT 21
#define MAX_TRANSMISSION_LENGTH 100000
#define MAX_THREAD_NUMBER 5
#define MAX_PATH 200

#define ERROR_SOCKET -3


class Server{

    private: 
        struct sockaddr_in server_address;
        void createServerSock();

        static Server server_instance;
        int server_port;
        std::string server_ip;
        std::string file_path;
        int server_sock;
        int data_socket;
        
    public:
        Server();
        Server(std::string, std::string);
        Server& operator = (const Server&);
        ~Server();

        // METHODS
        void initListen();
        void Start();
        void closeServer();

        // GETTERS
        std::string getFilePath();
        std::string getServerIP();
        int getServerPort();
        int getServerSocket();
        
        static Server* getInstance(){
            return &server_instance;
        }

        // SETTERS
        void setFilePath(std::string);
        void setServerPort(std::string);
        void closeData(int);
};

extern Server server;

#endif
