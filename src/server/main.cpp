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
#include "request.hpp"

int main(int argc, char** argv){

    // CREATE AND RUN THE SERVER
    if(argc != 3) {
        LOG_CRIT("Usage ./server PORT PATH");
        exit(1);
    }

    Server* server = Server::getInstance();
    server->setServerPort(argv[1]);
    server->setFilePath(argv[2]);
    server->Start();
    
    // CREATING THREADPOOL AND STARTING TO LOOK FOR JOBS
    ThreadPool thread_pool;
    

    LOG_INFO("Waiting for requests");
    // WHILE THE SERVER IS STILL RUNNING
    while(thread_pool.getExit() == false){

        // AND THE THE THREADPOOL IS NOT BUSY
        if(!thread_pool.busy()){
            // CREATE A CLIENT AND WAIT FOR A REQUEST TO BE SENT
            if(thread_pool.getAvailableThreads() > thread_pool.getNumThreads() / 2){
                thread_pool.QueueJob([]{
                    Ftp ftp_com;
                    if(ftp_com.listenRequest() == true){
                        Request request(ftp_com);
                        request.handle();
                    }});
            }
        }
    }
    
    // CLOSE ALL THREADS
    thread_pool.Stop();  

    // DISCONNECTS THE SERVER
    server->closeServer();
    
    return 0;
}
