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

#include "exception.hpp"
#include "server.hpp"
#include "client.hpp"
#include "threadPool.hpp"
#include "logger.hpp"

int main(int argc, char **argv){

    // CREATE AND RUN THE SERVER
    Server server;
    
    // CREATING THREADPOOL AND STARTING TO LOOK FOR JOBS
    ThreadPool tp;
    
    DBG_PRINT_LOGGER("Waiting for requests");
    // WHILE THE SERVER IS STILL RUNNING
    while(tp.getExit() == false){

        // AND THE THE THREADPOOL IS NOT BUSY
        if(tp.busy()){
            // CREATE A CLIENT AND WAIT FOR A REQUEST TO BE SENT
            tp.QueueJob([]{Client client;});
        }
    }
    
    // CLOSE ALL THREADS
    tp.Stop();  

    // DISCONNECTS THE SERVER
    server.closeFD();

    return 0;
}
