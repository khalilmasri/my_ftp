#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <mutex>
#include <thread>
#include <functional>
#include <condition_variable>


class ThreadPool {
public:
    void Start();
    void QueueJob(const std::function<void()>& job);
    void Stop();
    bool busy();
    void MainThread();
    void exit_listener();
    bool getExit();

private:
    void ThreadLoop();

    bool Exit = false;
    size_t THREADS_NUM;
    bool should_terminate = false;           // Tells threads to stop looking for jobs
    std::mutex queue_mutex;                  // Prevents data races to the job queue
    std::condition_variable mutex_condition; // Allows threads to wait on new jobs or termination 
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> jobs;
};

#endif
