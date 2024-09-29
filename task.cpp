#include "task.hpp"

// New task constructor
// @param id: Id of the new task, should be unique for each task
// @param job: function to call (lambda)
Task::Task(short id, const std::function<void()> & job) {
    if (id < 0) {
        logger.log("\t Invalid task ID", "Constructor");
        return;
    }
    logger.log("\t Task " + std::to_string(id) + " created", "Constructor");
    this->_id = id;
    this->_job = job;
    this->_status = Task::Status::READY;
}

// New task constructor, without task assigment
// @param id: Id of the new task, should be unique for each task
Task::Task(short id) {
    if (id < 0){
        logger.log("\t Invalid task ID", "Constructor");
        return;
    }
   logger.log("\t Task " + std::to_string(id) + " created", "Constructor");
    this->_id = id;
    this->_status = Task::Status::PLANNED;
}

// Task destructor
Task::~Task() {
    logger.log("\t Task " + std::to_string(_id) + " destroyed", "Destructor");
    this->_status = Task::Status::FINISHED;
    this->_job = NULL;
    this->_requires.clear();
}

// Allows to add task ID to "required tasks list"
// @param id: required task ID
void Task::addRequires(short id) {
    logger.log("\t Task " + std::to_string(_id) + " added required task " + std::to_string(id), "Add Required Tasks");
    this->_requires.push_back(id);
}

// Get Next required task ID
// @return last element of required queue
short Task::getRequires() {
    return this->_requires.back();
}

// Drops last element of required queue
void Task::popRequires() {
    if (this->_requires.size() > 0)
        this->_requires.pop_back();
}

// Release the task, updates status from PLANNED to READY
void Task::relase() {
    if (this->_status != Task::Status::PLANNED) return;
    logger.log("\t Task " + std::to_string(_id) + " relased!", "Release");
    this->_status = Task::Status::READY;
}

// Update task status to PENDING
void Task::start() {
    if (this->_status != Task::Status::READY) return;
    logger.log("\t Task " + std::to_string(_id) + " started!", "Start");
    this->_status = Task::Status::PENDING;
}

// Update task status to FINISHED
void Task::finish() {
    if (this->_status != Task::Status::PENDING) return;
    logger.log("\t Task " + std::to_string(_id) + " finished!", "Finish");
    this->_status = Task::Status::FINISHED;
}

// Manualy change status of task
// @param newStatus: new Task status
void Task::updateStatus(Task::Status newStatus) {
    this->_status = newStatus;
}

// Execute job (assigned lambda function)
void Task::run() {
    logger.log("\t Task " + std::to_string(_id) + " executed!", "Run Job");
    this->_job();
}

// Set current task job
// @param job: function to call (lambda)
void Task::setJob(const std::function<void()> & job) {
    this->_job = job;
}

// Get current task ID
// @return Current task ID
short Task::getID() {
    return this->_id;
};

// Get status of the current task
// @returns Task status
short Task::getStatus() {
    return this->_status;
};

// Get pointer to the current task status
// @return Pointer to task status
short * Task::getStatusAddress() {
    return &(this->_status);
}

// Checks if task is in status FINISHED
// @return True if is FINISHED, and False otherwise
bool Task::isDone() {
    return this->_status == Task::Status::FINISHED;
}

// Checks if task is in status PENDING
// @return True if is PENDING, and False otherwise
bool Task::isPending() {
    return this->_status == Task::Status::PENDING;
}

// Checks if task is in status READY
// @return True if is READY, and False otherwise
bool Task::isReady() {
    return this->_status == Task::Status::READY;
}

// Checks if task have any requirements
// @return True if queue is not empty, and False otherwise
bool Task::hasRequirements() {
    return !this->_requires.empty();
}

// Function to print debug information
void Task::printDebug() {
    std::cout << "Task Debug: ID: " <<  _id << " | Requirements: ";
    for (short singleReq : this->_requires) {
        std::cout << singleReq << " ";
    }
    std::cout << "| Status: " << this->_status << std::endl;
}
