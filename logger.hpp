#pragma once
#include <iostream>
#include <thread>
#include <memory>
#include <vector>
#include <functional>

using std::string;

// Logger class, to log runtime details
class Logger
{
private:
    std::string prefix = "";
    bool printLog = true;

public:
    Logger(std::string prefix, bool printLog = true);
    void log(std::string message);
    void log(std::string message, std::string caller);

    template <typename T>
    void logVector(const std::vector<T> vector, std::string caller, std::function<void(T)> print)
    {
        if (!this->printLog)
            return;
        
        std::cout << "\t========== Vector " << caller << "========== [Thread " << std::this_thread::get_id() << "]" << std::endl;
        for (T element : vector)
        {
            print(element);
        }
        std::cout << "\n\t========== Vector " << caller << "==========" << std::endl;
    };
};