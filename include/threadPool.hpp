#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <mutex>
#include <thread>

class ThreadPool {
    public:
        ThreadPool();
        ~ThreadPool();

        void Start();
        void QueueJob(const std::function<void()>& job);
        void Stop();
        void busy();

    private:
        void ThreadLoop();

        bool should_terminate = false;           // Tells threads to stop looking for jobs
        std::mutex queue_mutex;                  // Prevents data races to the job queue
        std::condition_variable mutex_condition; // Allows threads to wait on new jobs or termination 
        std::vector<std::thread> threads;
        std::queue<std::function<void()>> jobs;
};

#endif