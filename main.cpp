#include <iostream>
#include <thread>
#include <semaphore>
#include <mutex>
#include <vector>
#include <cstdlib>

#include "sort.hpp"
#include "threadPool.hpp"
//#include "logger.hpp"

using std::vector;
using std::mutex;
using std::thread;


int main()
{
    int randomNumbers = 100;
    srand(time(NULL));
    ThreadPool * pool = new ThreadPool(4);
    // { 1, 5, 2, 3, 4, 5, 7, 8 }
    mutex mtx;


    std::vector<int> arr { };

    for (int i = 0 ; i < randomNumbers ; i ++) {
        arr.push_back((rand() % 99) + 1);
    }

    int arrSize = arr.size();

    pool->enqueueTask([&arr, arrSize, &pool, &mtx] {
        Sort::mergeSort<int>(arr, 0, arrSize, *pool, mtx);
    });

    delete pool;

    std::cout << "\n\nResult..." << std::endl;
    for (int i : arr) {
        std::cout << i << std::endl;
    }
    return 0;
}

// Kamil 48269