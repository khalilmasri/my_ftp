#include "threadPool.hpp"
#include "logger.hpp"

bool detach = false;

ThreadPool::ThreadPool() {
    Start();
}

ThreadPool::~ThreadPool(){}

void ThreadPool::Start() {
    num_threads = std::thread::hardware_concurrency();  // Max # of threads the system supports
    available_threads = num_threads;
    threads.resize(num_threads);

    LOG_INFO("Creating thread pool...");

    for (uint32_t i = 0; i < num_threads; i++) {
        if (i == 0) {
            threads.at(i) = std::thread(&ThreadPool::exit_listener,this);
        }else{
            threads.at(i) = std::thread(&ThreadPool::ThreadLoop,this);
        }
    }

    LOG_DEBUG("Created %d threads", num_threads);
    LOG_INFO("Thread pool is ready!");
}

void ThreadPool::exit_listener() {

    std::string command;
    this->available_threads -= 1;
    while(command != "exit"){
        std::getline(std::cin, command);
    }

    LOG_INFO("Wrapping things up!");
    this->Exit = true;
}

bool ThreadPool::getExit() {
    return this->Exit;
}

void ThreadPool::ThreadLoop() {
    while (true) {
        int number;
        std::function<void()> job;
        {
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
        job();
        mutex_condition.notify_one();
    }
}

void ThreadPool::QueueJob(const std::function<void()>& job) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        available_threads-=1;
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
    return !poolbusy;
}

void ThreadPool::Stop() {

    LOG_INFO("Closing all threads...");
    std::unique_lock<std::mutex> lock(queue_mutex);
    should_terminate = true;

    lock.unlock();
    mutex_condition.notify_all();
    detach = true;
    for (std::thread& active_thread : threads) {
        active_thread.detach();
    }
    threads.clear();
    LOG_INFO("All threads are closed!");
}

int ThreadPool::getAvailableThreads(){
    return this->available_threads;
}

int ThreadPool::getNumThreads(){
    return this->num_threads;
}
