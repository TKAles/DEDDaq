// DEDLoggerConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include "MonoCamProducer.h"


MonoCamProducer TestProducer = MonoCamProducer();

int main()
{
    std::cout << "Testing Producer Code\n";
    std::cout << "Launching Test Thread\n";
    std::thread execution_thread([] {
        TestProducer.increment_run(); 
        });
    execution_thread.join();
    std::cout << TestProducer.times_ran << "\n";
    std::cout << "Thread finished. Terminating.";
    
}
