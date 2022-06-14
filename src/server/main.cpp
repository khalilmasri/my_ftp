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

#include "server.hpp"
#include "ftp.hpp"
#include "threadPool.hpp"
#include "logger.hpp"

int main(void){

    // CREATE AND RUN THE SERVER
    Server server;
    
    // CREATING THREADPOOL AND STARTING TO LOOK FOR JOBS
    ThreadPool thread_pool;
    

    LOG_INFO("Waiting for requests");
    // WHILE THE SERVER IS STILL RUNNING
    while(thread_pool.getExit() == false){

        // AND THE THE THREADPOOL IS NOT BUSY
        if(thread_pool.busy()){
            // CREATE A CLIENT AND WAIT FOR A REQUEST TO BE SENT
            thread_pool.QueueJob([]{Ftp request;});
        }
    }
    
    // CLOSE ALL THREADS
    thread_pool.Stop();  

    // DISCONNECTS THE SERVER
    server.closeServer();

    return 0;
}
