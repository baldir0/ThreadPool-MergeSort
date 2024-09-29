#pragma once

#include <mutex>
#include <iostream>
#include <functional>
#include <vector>
#include <deque>

#include "logger.hpp"


// Single task object class
class Task {
    private:
    short _id;
    std::function<void()> _job = NULL;
    std::deque<short> _requires{ };
    Logger logger = Logger("Task", true);
    short _status = this->READY;

    void printDebug();

    public:
    Task(short id, const std::function<void()> & job);
    Task(short id);
    Task() = default;
    ~Task();

    enum Status {
        PLANNED,
        READY,
        PENDING,
        FINISHED
    };

    void addRequires(short id);
    short getRequires();
    void popRequires();

    void relase();
    void start();
    void finish();
    void updateStatus(Task::Status newStatus);

    void run();
    void setJob(const std::function<void()> & job);
    
    short getID();
    short getStatus();
    short *getStatusAddress();

    bool isDone();
    bool isPending();
    bool isReady();
    bool hasRequirements();

};