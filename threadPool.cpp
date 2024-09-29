#include "threadPool.hpp"

// Threadpool constructor
// @param threadNo: number of threads
ThreadPool::ThreadPool(short threadsNo)
{
    this->logger.log("Pool Created", "Constructor");
    for (short i = 0; i < threadsNo; i++)
    {
        this->logger.log("Thread Created", "Constructor");
        this->activeThreads.emplace_back(
            std::thread([this]
                        { this->threadLoop(); }));
    }
};

// Threadpool destructor
ThreadPool::~ThreadPool()
{
    this->logger.log("Stopping Pool", "Destructor");
    this->stopPool();
    cv.notify_all();
    for (std::thread &singleThread : activeThreads)
    {
        this->logger.log("Thread joined", "Destructor");
        if (singleThread.joinable())
            singleThread.join();
    }
};

// Main loop of threadpool, runs in loop and call tasks from the queue
void ThreadPool::threadLoop()
{
    logger.log("Loop initialized", "Thread Loop");
    while (true)
    {
        Task *taskHanlder = nullptr;
        int taskID = 0;
        {
            std::unique_lock<std::mutex> lock(queueLock);
            cv.wait(lock, [this]
                    { return !this->queue.empty() || this->_stopPool; });
            if (this->queue.empty() && this->_stopPool)
                return;
            printQueue();
            taskHanlder = getNextTask();
            if (taskHanlder == nullptr)
                continue;
            taskID = taskHanlder->getID();
            std::move(this->queue.at(getCurrentTaskIndex(taskID)))->start();
        }
        std::move(this->queue.at(getCurrentTaskIndex(taskID)))->run();
        {
            std::unique_lock<std::mutex> lock(queueLock);
            std::move(this->queue.at(getCurrentTaskIndex(taskID)))->finish();
            this->clearQueue();
        }
    }
};

// Function to print queue elements and status
void ThreadPool::printQueue()
{
    logger.logVector<Task *>(this->queue, "Queue", [this](Task *task)
                             { std::cout << "[" << task->getID() << "|" << task->getStatus() << "] "; });
}

// Get single task pointer
// @req lookupID: ID of task to get
// @return pointer to task or nullptr if there is no tast with passed ID
Task *ThreadPool::getTask(short lookupID)
{
    for (Task *singleTask : this->queue)
    {
        if (singleTask->getID() == lookupID)
            return singleTask;
    }
    return nullptr;
}

// Get next ready task
// @return pointer to task or nullptr if there is no ready task in queue
Task *ThreadPool::getNextTask()
{
    for (Task *singleTask : this->queue)
    {
        // singleTask->printDebug();

        if (!singleTask->isReady())
            continue;
        if (singleTask->hasRequirements()               // If task has requirements
            && !checkTask(singleTask->getRequires()))   // and required task is not in queue
        { 
            singleTask->popRequires();
            return nullptr;
        }
        if (singleTask->hasRequirements())
            continue;

        return singleTask;
    }

    return nullptr;
}

// Get index of task with passed ID
// @return index of task with passed ID or -1 if not found
int ThreadPool::getCurrentTaskIndex(int lookupID)
{
    int index = 0;
    for (Task *singleTask : this->queue)
    {
        if (singleTask->getID() == lookupID)
            return index;
        index++;
    }
    return -1;
}

// Check if there is at least one task in READY status
// @return True if there is at least one task in READY
bool ThreadPool::hasReadyTasks()
{
    // std::unique_lock<std::mutex> lock(queueLock);
    for (Task *singleTask : this->queue)
    {
        logger.log("Task: " + std::to_string(singleTask->getID()) +
                       " | Status: " + std::to_string(singleTask->getStatus()) +
                       " | Requirements: " + std::to_string(singleTask->getRequires()),
                   "Has Ready Tasks?");
        if (singleTask->isReady() && singleTask->getRequires() == -1)
        {
            logger.log("Task " + std::to_string(singleTask->getID()) + " is ready to launch", "Has Ready Tasks?");
            return true;
        }
    }
    logger.log("Tasks are not ready!", "Has Ready Tasks?");
    return false;
}

// Function to check if task with passed ID is in the queue or not
// @return True if task is in the queue
bool ThreadPool::checkTask(int taskID)
{
    for (Task *singleTask : this->queue)
    {
        if (singleTask->getID() == taskID)
            return true;
    }

    return false;
}

// Function to clear queue of FINISHED tasks
// It has to stop after each deletion because of memory address change
// @return True if delete any task
bool ThreadPool::clearQueue()
{
    int index = 0;
    for (Task *singleTask : this->queue)
    {
        if (singleTask->isDone())
        {
            logger.log("Task status: " + std::to_string(singleTask->getStatus()) + "", "Queue Cleaner");
            logger.log("Removing Task " + std::to_string(singleTask->getID()) + " from queue!", "Queue Cleaner");
            if (this->queue.size() > index)
            {
                delete singleTask;
                this->queue.erase(this->queue.begin() + index);
            }
            return true;
        }
        index++;
    }
    return true;
}

// Function to add empty tasks into the queue
// @return ID of created task
short ThreadPool::enqueueTask()
{
    std::unique_lock<std::mutex> lock(queueLock);
    short nextId = this->_lastID + 1;
    logger.log("Enqueue task with id " + std::to_string(nextId), "Enqueue task");
    queue.push_back(new Task(nextId));
    this->_lastID = nextId;
    return nextId;
}

// Function to add tasks into the queue
// @return ID of created task
short ThreadPool::enqueueTask(const std::function<void()> &job)
{
    std::unique_lock<std::mutex> lock(queueLock);
    short nextId = this->_lastID + 1;
    logger.log("Enqueue task with id " + std::to_string(nextId), "Enqueue task");
    queue.push_back(new Task(nextId, job));
    this->_lastID = nextId;
    this->cv.notify_one();
    return nextId;
}

// Function to add tasks into the queue with requirements
// @return ID of created task
short ThreadPool::enqueueTask(const std::function<void()> &job, std::vector<short> requirements)
{
    std::unique_lock<std::mutex> lock(queueLock);
    short nextId = this->_lastID + 1;
    logger.log("Enqueue task with requirements and id " + std::to_string(nextId), "Enqueue task");
    Task *task = new Task(nextId, job);

    for (std::vector<short>::iterator it = requirements.begin(); it < requirements.end(); it++)
    {
        logger.log("Requirement add to task " + std::to_string(nextId), "Enqueue task");
        task->addRequires((*it));
    }

    queue.push_back(task);
    this->_lastID = nextId;
    this->cv.notify_one();
    return nextId;
}

// Function to stop the thread pool
void ThreadPool::stopPool()
{
    std::unique_lock<std::mutex> lock(queueLock);
    this->_stopPool = true;
}