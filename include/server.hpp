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
#define SERVER_PORT 8080
#define MAX_TRANSMISSION_LENGTH 100000
#define MAX_THREAD_NUMBER 256

#define ERROR_SOCKET -3

extern int server_sock;

class Server{

    private: 
        struct sockaddr_in server_address;
        int addToThreads();
        int handleClient();
        void create_server_sock();
        bool     create_thread_pool(void);

    public:
        Server();
        Server& operator = (const Server&);
        ~Server();

        // METHODS
        void run();
        void closeFD();
        int getServerSockID();
};


#endif
