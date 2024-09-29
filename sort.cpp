#include "sort.hpp"

// template <typename T>
// void Sort::merge(std::vector<T> &arr, unsigned int arrStart, unsigned int arrMiddle, unsigned int arrEnd, std::mutex & mtx)
// {
//     Logger logger = Logger("SORT - MERGE", false);
//     std::unique_lock<std::mutex> lock(mtx);
//     logger.log("Array Start: " + std::to_string(arrStart), "Merge Args Value");
//     logger.log("Array Middle: " + std::to_string(arrMiddle), "Merge Args Value");
//     logger.log("Array End: " + std::to_string(arrEnd), "Merge Args Value");

//     int i = arrStart;
//     int k = 0;
//     int j = arrMiddle;

//     T *outputArr = new T[arrEnd - arrStart + 1];

//     while ( i < arrMiddle && j < arrEnd) {
//         logger.log("LEFT: (Index: " + std::to_string(i) + ", Value: " + std::to_string(arr[i]) + ") | RIGHT: (Index: " + std::to_string(j) + ", Value: " + std::to_string(arr[j]) + ")" , "MERGE COMPARATION");
//         if (arr[i] < arr[j]) {
//             logger.log("LEFT WIN", "MERGE COMPARATION");
//             outputArr[k] = arr[i];
//             i++;
//         } else {
//             logger.log("RIGHT WIN", "MERGE COMPARATION");
//             outputArr[k] = arr[j];
//             j++;
//         }
//         logger.log("CURRENT K VALUE: " + std::to_string(outputArr[k]));
//         k++;
//     }

//     while (i < arrMiddle) {
//         logger.log("Writing rest of I: " + std::to_string(arr[i]) + ", Current J: " + std::to_string(i), "MERGE COMPARATION");
//         outputArr[k] = arr[i];
//         i++;
//         k++;
//     }

//     while (j < arrEnd) {
//         logger.log("Writing rest of J: " + std::to_string(arr[j]) + ", Current J: " + std::to_string(j), "MERGE COMPARATION");
//         outputArr[k] = arr[j];
//         j++;
//         k++;
//     }

//     logger.log("Overwriting original Array Value", "Merge LOG");
//     for (int pos = arrStart, k = 0; pos < arrEnd; pos++, k++)
//     {
//         logger.log("Pos Index: " + std::to_string(pos) + ", Value: " + std::to_string(arr[pos]) + " | K Index: " + std::to_string(k) + ", Value: " + std::to_string(outputArr[k]), "Merge - Overwrite");

//         arr[pos] = outputArr[k];
//     }

//     delete[] outputArr;
// }

// template <typename T>
// void Sort::mergeSort(std::vector<T> &arr, unsigned int arrStart, unsigned int arrEnd, ThreadPool &pool, std::mutex &mtx, short prevMergeId, short parent)
// {
//     Logger logger = Logger("MergeSort", false);
//     int arrMiddle = ((arrStart + arrEnd) / 2);
//     logger.log(std::to_string(arrStart), "Split | Start Value");
//     logger.log(std::to_string(arrMiddle), "Split | Middle Value");
//     logger.log(std::to_string(arrEnd), "Split | End Value");

//     if (arrEnd <= arrStart + 1) {
//         logger.log("Break occured", "Split | Break Condition");   
//         return;
//     }
    
//     logger.log("Create empty task", "Split | End Value");
//     short MergeID = pool.enqueueTask();
//     short firstSplitID = pool.enqueueTask();
//     short secondSplitID = pool.enqueueTask();

//     if (prevMergeId != -1 && pool.getTask(prevMergeId) != nullptr) pool.getTask(prevMergeId)->addRequires(MergeID);
//     if (parent != -1 && pool.getTask(parent) != nullptr) pool.getTask(parent)->addRequires(MergeID);

//     logger.log("Enqueu first Split", "Split | End Value");
//     pool.getTask(firstSplitID)->setJob([&arr, &pool, &mtx, arrStart, arrMiddle, MergeID, firstSplitID] {
//         mergeSort<T>(arr, arrStart, arrMiddle, pool, mtx, MergeID, firstSplitID);
//     });
//     logger.log("Second Split | ArrStart: " + std::to_string(arrMiddle) + " | ArrMiddle/END: " + std::to_string(arrEnd) + " | Task ID: " + std::to_string(secondSplitID) + " | Prev Merge ID: " + std::to_string(prevMergeId) + " | Parent ID: " + std::to_string(parent), "Split | Second Task Result") ;

//     pool.getTask(MergeID)->addRequires(firstSplitID);
//     if (prevMergeId != -1 && pool.getTask(prevMergeId) != nullptr) pool.getTask(prevMergeId)->addRequires(firstSplitID);
//     if (parent != -1 && pool.getTask(parent) != nullptr) pool.getTask(parent)->addRequires(firstSplitID);

//     logger.log("Enqueu second Split", "Split | End Value");
//     pool.getTask(secondSplitID)->setJob([&arr, &pool, &mtx, arrEnd, arrMiddle, MergeID, secondSplitID] {
//         mergeSort<T>(arr, arrMiddle, arrEnd, pool, mtx, MergeID, secondSplitID);
//     });
//     logger.log("Second Split | ArrStart: " + std::to_string(arrMiddle) + " | ArrMiddle/END: " + std::to_string(arrEnd) + " | Task ID: " + std::to_string(secondSplitID) + " | Prev Merge ID: " + std::to_string(prevMergeId) + " | Parent ID: " + std::to_string(parent), "Split | Second Task Result") ;

//     pool.getTask(MergeID)->addRequires(secondSplitID);
//     if (prevMergeId != -1 && pool.getTask(prevMergeId) != nullptr) pool.getTask(prevMergeId)->addRequires(secondSplitID);
//     if (parent != -1 && pool.getTask(parent) != nullptr) pool.getTask(parent)->addRequires(secondSplitID);

//     logger.log("Set task job", "Split | End Value");
//     pool.getTask(MergeID)->setJob([&arr, &mtx, arrStart, arrEnd, arrMiddle, &pool] {
//         merge<T>(arr, arrStart, arrMiddle, arrEnd, mtx);
//     });

//     logger.log("Release task", "Split | End Value");

//     pool.getTask(firstSplitID)->relase();
//     pool.getTask(secondSplitID)->relase();
//     pool.getTask(MergeID)->relase();

//     logger.log("Merge | ArrStart: " + std::to_string(arrStart) + " | ArrMiddle/END: " + std::to_string(arrEnd) + " | Task ID: " + std::to_string(MergeID) + " | Prev Merge ID: " + std::to_string(prevMergeId) + " | Parent ID: " + std::to_string(parent), "Split | Merge Task Result") ;
// }

// template void Sort::mergeSort<int>(std::vector<int> &arr, unsigned int arrStart, unsigned int arrEnd, ThreadPool & pool, std::mutex & mtx, short prevMergeId, short parent);