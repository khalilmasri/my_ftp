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

int main(int argc, char **argv){

    /* try{ */
    /*     if(argc!=1) */
    /*         throw(ArgsException()); */
    /* }catch(ArgsException &err){ */
    /*     std::cout << err.what() << std::endl; */
    /*     exit(-1); */
    /* } */

    Server server;
    server.run();
    while (1)
    {
        if (server.getNewClient())
            server.handleClient();
    }
    return 0;
}

