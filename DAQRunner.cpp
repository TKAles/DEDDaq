#include "DAQRunner.h"
#include <vector>
#include <string>
#include <iostream>
#include "VimbaCPP/Include/VimbaSystem.h"

int DAQRunner::startupAVSystem()
{
    std::cout << "AlliedVision Startup" << std::endl;
    CameraPtrVector _cameraPtrs;
    std::string _tmpDeviceID;
    if (VmbErrorSuccess == DAQRunner::cameraSystem.Startup())
    {
        std::cout << "Initialized Vimba Driver" << std::endl;

        // Try and get the names of the connected cameras
        if (VmbErrorSuccess == DAQRunner::cameraSystem.GetCameras(_cameraPtrs))
        {
            std::cout << "Found Cameras!" << std::endl;
            // Clear the old camera list if there was one
            DAQRunner::AVCameraIDs.clear();
            // Create an iterator to grab all the device IDs
            // so we can create individual camera objects
            for (CameraPtrVector::iterator iter = _cameraPtrs.begin();
                _cameraPtrs.end() != iter; iter++)
            {
                // Try and grab a camera pointer
                if (VmbErrorSuccess == (*iter)->GetName(_tmpDeviceID))
                {
                    DAQRunner::AVCameraIDs.push_back(_tmpDeviceID);
                }
                else {
                    std::cout << "Something went wrong grabbing a camera pointer!" << std::endl;
                    return -3;
                }
            }

        }
        else {
            std::cout << "Something went wrong trying to access the cameras!" << std::endl;
            return -2;
        }
    }
    else {
        std::cout << "Error attempting to initialize driver!" << std::endl;
        return -1;
    }

    return 0;
}
int DAQRunner::findAVCameras()
{
    std::cout << "findAVCameras Stub!" << std::endl;
    // TODO: Add your implementation code here.
    return 0;
}

int DAQRunner::makeAVC(std::string _camID)
{
    std::cout << "makeAVC Stub!" << std::endl;
    // TODO: Add your implementation code here.
    return 0;
}


int DAQRunner::setupCapture(int _ptsToCap, float _freqToCap)
{
    std::cout << "setupCapture Stub!" << std::endl;
    // TODO: Add your implementation code here.
    return 0;
}
