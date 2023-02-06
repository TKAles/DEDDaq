// DEDLoggerConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include "MonoCamProducer.h"


MonoCamProducer TestProducer = MonoCamProducer();

int main()
{
    std::cout << "Starting up Allied Vision Subsystem.\n";
    TestProducer.startupCamera();
    std::cout << "Looking for connected USB cameras\n";
    TestProducer.registerCameras();
    
    TestProducer.shutdownCamera();
    
}
