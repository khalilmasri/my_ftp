#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
     #include <netdb.h>
#define SERVER_PORT 8080
#define SERVER_RES_MAX_LENGTH 256
#define PATH_TO_SOCKET "/tmp"
#define PATH_TO_SOCKET_LENGTH strlen(PATH_TO_SOCKET)

#define NO_ARG -1 
#define SOCKET_ALLOCATION_FAILED -2
#define MEM_ALLOC_FAILED -3
#define BIND_FAILED -4
#define CONNECTION_FAILED -5
#define MAX_CMD_SIZE 100
#endif