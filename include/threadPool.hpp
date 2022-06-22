#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <mutex>
#include <thread>
#include <functional>
#include <condition_variable>
#include <iostream>
#include <string>

extern bool detach;
class ThreadPool {
public:
    ThreadPool();
    ~ThreadPool();
    void Start();
    void QueueJob(const std::function<void()>& job);
    void Stop();
    bool busy();
    void MainThread();
    void exit_listener();
    bool getExit();
    int getAvailableThreads();
    int getNumThreads();

private:
    void ThreadLoop();

    bool Exit = false;
    int num_threads;
    bool should_terminate = false;           // Tells threads to stop looking for jobs
    int available_threads;
    std::vector<int> thread_ID;
    std::mutex queue_mutex;                  // Prevents data races to the job queue
    std::condition_variable mutex_condition; // Allows threads to wait on new jobs or termination 
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> jobs;
};

#endif
