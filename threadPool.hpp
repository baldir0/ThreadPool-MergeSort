#pragma once

#include <iostream>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <queue>
#include <vector>
#include <list>
#include "task.hpp"
#include "logger.hpp"

// Thread pool controler class
class ThreadPool {
    private:
    std::mutex queueLock;
    std::vector<Task*> queue;
    std::vector<std::thread> activeThreads;
    std::condition_variable cv;
    bool _stopPool = false;
    short _lastID = 0;

    Logger logger = Logger("ThreadPool", true);  // Switch to true, to print log
    
    void threadLoop();
    void printQueue();

    Task * getNextTask();
    int getCurrentTaskIndex(int lookupID);

    bool hasReadyTasks();
    bool checkTask(int taskID);
    bool clearQueue();

    public:
    ThreadPool(short threadsNo = 1);
    ~ThreadPool();
    
    Task * getTask(short lookupID);

    short enqueueTask();
    short enqueueTask(const std::function<void()> & job);
    short enqueueTask(const std::function<void()> & job, std::vector<short> requirements);

    void stopPool();
};