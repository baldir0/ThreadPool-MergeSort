#include "logger.hpp"

// Constructor of logger object
// @req prefix: prefix of log message
// @req printLog: set to true, to print log messages
Logger::Logger(std::string prefix, bool printLog) {
    this->prefix = prefix;
    this->printLog = printLog;
};

// Basic log function
// @req message: message to print
void Logger::log(std::string message) {
    if (!this->printLog) return;
    std::cout << "[Thread " << std::this_thread::get_id() << " | " << this->prefix << "] - " << message << std::endl;
}

// Basic log function with caller param
// @req message: message to print
// @req caller: additional prefix to add
void Logger::log(std::string message, std::string caller) {
    if (!this->printLog) return;
    std::cout << "[Thread " << std::this_thread::get_id() << " | " << this->prefix << " | " << caller << "] - " << message << std::endl;
}
