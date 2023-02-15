// DEDLoggerConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include "MonoCamProducer.h"
#include "MonoFrameObserver.h"
#include "IRFrameProducer.h"
#include "sdk/IRLogger.h"

int main()
{
    MonoCamProducer TestProducer = MonoCamProducer();
    const char logPath[] = "D:\\IRLog.txt";
    evo::IRLogger::setVerbosity(evo::IRLOG_DEBUG, evo::IRLOG_OFF, logPath);
    IRFrameProducer TestIRProducer = IRFrameProducer();
    std::cout << "Starting up Allied Vision Subsystem." << std::endl;
    TestProducer.startupCamera();
    std::cout << "Looking for connected USB cameras" << std::endl;
    TestProducer.registerCameras();
    std::cout << "Attempting to initialize the IR Camera." << std::endl;

    TestProducer.shutdownCamera();
    
}
