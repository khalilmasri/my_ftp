#include "includes/system.h"
static int client_socket;

int getCommandId(char *s)
{
    if (!strncmp(s, "sendFile", 8))
        return 1;
    return (0);
}

int sendFileToServ(char *arg)
{
    printf("PAS1\n");
    //int fd = open(arg, 'r'); 
    int fd = send(client_socket, "ijoj", 5, 0);
    printf("%d\n", fd);
    return (0);
}

int analyse_buffer(char *s)
{
    int cmd = getCommandId(s);
    printf("SERV CMD %s\n", s);
    switch (cmd) {
        case 1:
            sendFileToServ(s + 9);
    }
    return (0);
}

int         connectToServer(char *url)
{
    printf("%s\n%d\n", url, SERVER_PORT);
    struct sockaddr_in client;

    int charRead;

    char server_res[SERVER_RES_MAX_LENGTH + 1];
    //create the socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return SOCKET_ALLOCATION_FAILED;

    client.sin_family = AF_INET;
    client.sin_port = SERVER_PORT;
    client.sin_addr.s_addr = inet_addr(url);

    int connect_socket = connect(client_socket, (struct sockaddr *) &client, sizeof(client));
    if (connect_socket < 0)
        return CONNECTION_FAILED;
    int readBytes;


    while ((readBytes = read(1, server_res, SERVER_RES_MAX_LENGTH - 1)) > 0)
    {
        server_res[SERVER_RES_MAX_LENGTH] = '\0';

        printf("%s\n", server_res);
        analyse_buffer(strdup(server_res));
    }
  
    return (0);
}

int         main(int ac, char **av)
{
    int error_code;

    char deafaultUrl[] = "127.0.0.1"; //~fdc/sample.html
    ac != 2 ? (error_code = connectToServer(deafaultUrl)) : (error_code = connectToServer(av[1]));
     if ( error_code < 0)
    {
        printf("error occured with code %d", error_code);
        return error_code;
    }
    printf("clean exit()\n");
    return (0);
}