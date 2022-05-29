#include "../include/threadPool.hpp"
#include "../include/server.hpp"

// ThreadPool::ThreadPool() {}

void ThreadPool::Start() {
    const uint32_t num_threads = (std::thread::hardware_concurrency()/2) + 1;  // Max # of threads the system supports
    threads.resize(num_threads);
    for (uint32_t i = 0; i < num_threads; i++) {
        if (i == 0) {
            threads.at(i) = std::thread(&ThreadPool::exit_listener,this);
        }else{
            threads.at(i) = std::thread(&ThreadPool::ThreadLoop,this);
        }
    }

}

void ThreadPool::exit_listener() {

    std::string command;
    while(command != "exit"){
        std::getline(std::cin, command);
    }

    this->Exit = true;
}

bool ThreadPool::getExit() {
    return this->Exit;
}
void ThreadPool::ThreadLoop() {
    while (true) {
        int number;
        std::function<void(int n)> job;
        {
            int num = n;
            std::unique_lock<std::mutex> lock(queue_mutex);
            mutex_condition.wait(lock, [this] {
                return !jobs.empty() || should_terminate;
            });
            if (should_terminate) {
                return;
            }

            job = jobs.front();
            jobs.pop();
        }
        job(this->THREADS_NUM);
    }
}


void ThreadPool::QueueJob(const std::function<void(int)>& job) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        jobs.push(job);
    }
    mutex_condition.notify_one();
}

bool ThreadPool::busy() {
    bool poolbusy;
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        poolbusy = jobs.empty();
    }
    return poolbusy;
}

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