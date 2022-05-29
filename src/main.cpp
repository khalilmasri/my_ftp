/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description: my_ftps
 *
 *        Version:  1.0
 *        Created:  04/24/2022 03:24:22 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Khalil Masree, Chris Dallat
 *   Organization:  European Leadership univirsity 
 *
 * =====================================================================================
 */

#include "../include/exception.hpp"
#include "../include/server.hpp"
#include <string>
#include "../include/threadPool.hpp"


int main(int argc, char **argv){

    Server server;
    server.run();

    ThreadPool Th;
    Th.Start();

    
    while(!Th.getExit()){
            // int request_accept;
            // struct sockaddr_in client_addr;
            // socklen_t addr_size = sizeof(client_addr);
            // if ((request_accept = accept(0, (struct sockaddr*)&client_addr, &addr_size)) < 0)
            // {
            //     std::cout << "accept Error\n";
            //     exit(-1);
            // }
            int num = 1;
            /* std::function<void(int)> job = [](int num){return num;}; */
            /* Th.QueueJob(job); */
    }

     Th.Stop();  

    return 0;
}
