#include "../include/threadPool.hpp"
#include "../include/server.hpp"

ThreadPool::ThreadPool() {}

ThreadPool::ThreadPool(const int server_sock) {
    const uint32_t num_threads = std::thread::hardware_concurrency() / 2; // Max # of threads the system supports
    threads.resize(num_threads);
    this->server_sock = server_sock;
    for (uint32_t i = 0; i < num_threads; i++) {
        threads.push_back(std::thread(&ThreadPool::ThreadLoop, this));
    }

    std::cout << "num of thread created = " << num_threads << std::endl;
    
    for(int i = 0; i < num_threads; i++){
        std::cout << "JOINING\n";
        threads.at(i).join();
    }
}

void ThreadPool::ThreadLoop() {
    while (true) {
        //The anonymous code blocks are used so that when they are exited, 
        //the std::unique_lock variables created within them go out of scope, unlocking the mutex.
        std::function<void()> job;
        {
            int bytes;
            char buff[MAX_TRANSMISSION_LENGTH];
            int request_accept;
            struct sockaddr_in client_addr;
            socklen_t addr_size = sizeof(client_addr);
            int count = 0;
            std::cout << "HERE MALAKA\n";

            if ((request_accept = accept(this->server_sock, (struct sockaddr*)&client_addr, &addr_size)) < 0)
            {
                std::cout << "accept Error\n";
                exit(-1);
            }
            std::cout << "IM AFTER ACCEPT\n>";
            std::cout << "Client Addr = " << client_addr.sin_addr.s_addr << std::endl;

            bytes = read(request_accept, &buff, MAX_TRANSMISSION_LENGTH);

            std::unique_lock<std::mutex> lock(queue_mutex);
            mutex_condition.wait(lock, [this] {
                    return !jobs.empty() /*|| should_terminate*/;
                    });
            /* if (should_terminate) { */
            /*     return; */
            /* } */
            job = jobs.front();
            jobs.pop();
        }
        //lock destroyed
        job();
    }
}

ThreadPool::~ThreadPool() {};

void ThreadPool::QueueJob(const std::function<void()>& job) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        jobs.push(job);
    }
    mutex_condition.notify_one();
};

void ThreadPool::Stop() {  
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        should_terminate = true;
    }
    mutex_condition.notify_all();
    for (std::thread& active_thread : threads) {
        active_thread.join();
    }
    threads.clear();
}

bool ThreadPool::busy() {  
    bool poolbusy = false;
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        poolbusy = jobs.empty();
    }
    return poolbusy;
}

