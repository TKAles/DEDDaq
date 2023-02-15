// DEDLoggerConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <iostream>
#include <thread>
#include "MonoCamProducer.h"
#include "MonoFrameObserver.h"
#include "IRFrameProducer.h"
#include "sdk/IRLogger.h"

const int FRAMESTOGRAB = 250;
std::vector<std::thread> monochromeThreadProducers;

int main()
{
    // Information for the evocortex logging library
    const char logPath[] = "D:\\IRLog.txt";
    // Set stdout to INFO level and set the file log to OFF level.
    // Still have to pass it a path though.
    evo::IRLogger::setVerbosity(
        evo::IRLOG_INFO, 
        evo::IRLOG_OFF, logPath);

    // Initialize the monochrome camera producer object and
    // its frame grabber
    MonoCamProducer MonoProducer = MonoCamProducer();
    MonoProducer.startupCamera();
    MonoProducer.registerCameras();
    MonoProducer.shutdownCamera();
    
}
