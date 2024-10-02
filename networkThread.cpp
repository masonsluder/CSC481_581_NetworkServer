#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <iostream>

#include "networkthread.h"

Thread::Thread(int i, Thread* other, std::mutex* _mutex, std::condition_variable* _condition_variable)
{
    // Sets the thread ID, 
    this->i = i;
    if (i == 0) { busy = true; }
    else { this->other = other; }
    this->_mutex = _mutex;
    this->_condition_variable = _condition_variable;
}

bool Thread::isBusy()
{
    std::lock_guard<std::mutex> lock(*_mutex);  // this locks the mutex until the variable goes out of scope (i.e., when the function returns in this case)
    return busy;
}

void Thread::run()
{
    // Client threads
    std::cout << "New thread created for client.\n";
}


/**
 * Wrapper function because threads can't take pointers to member functions.
 */
void run_wrapper(Thread* fe)
{
    fe->run();
}

//int main()
//{
//    // Mutex to handle locking, condition variable to handle notifications between threads
//    std::mutex m;
//    std::condition_variable cv;
//
//    // Create thread objects
//    NetworkThread t1(0, NULL, &m, &cv);
//    NetworkThread t2(1, &t1, &m, &cv);
//
//    std::cout << "Starting Thread.cpp\n";
//
//    // Start threads
//    std::thread first(run_wrapper, &t1);
//    std::thread second(run_wrapper, &t2);
//
//    // Make sure both threads are complete before stopping main thread
//    first.join();
//    second.join();
//}
