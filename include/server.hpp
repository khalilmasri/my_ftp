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
#define MAX_TRANSMISSION_LENGTH 100000


#define ERROR_SOCKET -3
class Server{

    private: 
        int server_sock;
        struct sockaddr_in server_address;
        std::vector<Client> clients;
        int client_number;
        std::vector<int> fd;
        bool newClient;
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
};


#endif
