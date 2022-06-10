#include "threadPool.hpp"
#include "logger.hpp"

ThreadPool::ThreadPool() {
    Start();
}

void ThreadPool::Start() {
    const uint32_t num_threads = std::thread::hardware_concurrency();  // Max # of threads the system supports
    threads.resize(num_threads);

    DBG_PRINT_LOGGER("Creating thread pool...");

    for (uint32_t i = 0; i < num_threads; i++) {
        if (i == 0) {
            threads.at(i) = std::thread(&ThreadPool::exit_listener,this);
        }else{
            threads.at(i) = std::thread(&ThreadPool::ThreadLoop,this);
        }
    }

    DBG_PRINT_LOGGER("Thread pool is ready!");
}

void ThreadPool::exit_listener() {

    std::string command;
    while(command != "exit"){
        std::getline(std::cin, command);
    }

    DBG_PRINT_LOGGER("Wrapping things up!");
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

    DBG_PRINT_LOGGER("Closing all threads...");
    std::unique_lock<std::mutex> lock(queue_mutex);
    should_terminate = true;

    lock.unlock();
    mutex_condition.notify_all();
    for (std::thread& active_thread : threads) {
        active_thread.detach();
    }
    threads.clear();
    DBG_PRINT_LOGGER("All threads are closed!");
}
