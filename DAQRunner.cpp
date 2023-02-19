#include "DAQRunner.h"
#include <vector>
#include <string>
#include <iostream>
#include "VimbaCPP/Include/VimbaSystem.h"
#include "AVCameraObject.h"

// Constructor 
DAQRunner::DAQRunner()
{
    // Make starting the vision system up part of the 
    // constructor.
    DAQRunner::cameraSystem.Startup();
}

// Destructor
DAQRunner::~DAQRunner()
{
    // Make shutdown part of the destructor to ensure it gets
    // properly released every time
    DAQRunner::cameraSystem.Shutdown();
}

// Seeks connected cameras and spawns AVCameraObjects
// with the discovered ID.
int DAQRunner::startupAVSystem()
{
    CameraPtrVector _cameraPtrs;
    std::string _tmpDeviceID;
        // Try and get the names of the connected cameras
        if (VmbErrorSuccess == DAQRunner::cameraSystem.GetCameras(_cameraPtrs))
        {
            // Clear the old camera list if there was one
            DAQRunner::AVCameraIDs.clear();
            // Create an iterator to grab all the device IDs
            // so we can create individual camera objects
            for (CameraPtrVector::iterator iter = _cameraPtrs.begin();
                _cameraPtrs.end() != iter; iter++)
            {
                // Try and grab a camera pointer
                if (VmbErrorSuccess == (*iter)->GetID(_tmpDeviceID))
                {
                    DAQRunner::AVCameraIDs.push_back(_tmpDeviceID);
                    auto _tmpCam = AVCameraObject(_tmpDeviceID);
                    DAQRunner::assocAVCameras.push_back(_tmpCam);
                    
                } else {
                    std::cout << 
                        "Something went wrong grabbing a camera pointer!" << std::endl;
                    return -2;
                }
            }

            std::cout << "Finished populating camera device IDs." << std::endl;
            return 0;
        } else {
            std::cout <<  "Something went wrong trying to access the cameras!" << std::endl;
            return -1;
        }

}

int DAQRunner::configureMonochromeSettings()
{
    std::string _tmpFeature;
    std::string _tmpValue;
    for (auto camiter = std::begin(DAQRunner::assocAVCameras);
        camiter != std::end(DAQRunner::assocAVCameras); camiter++)
    {
        _tmpFeature = "ExposureTime";
        _tmpValue = "5000.0";
        auto result = camiter->configureFeature(_tmpFeature, 
            _tmpValue, DAQRunner::cameraSystem);
        if (result != 0)
        {
            std::cout << "Something went wrong setting the Exposure Time!" << std::endl;
            return -1;
        }
        _tmpFeature = "Gain";
        _tmpValue = "24.0";
        result = camiter->configureFeature(_tmpFeature, _tmpValue,
            DAQRunner::cameraSystem);
        if (result != 0)
        {
            std::cout << "Something went wrong setting the  Amplifier Gain!" << std::endl;
            return -2;
        }
    }
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

int DAQRunner::shutdownAVSystem()
{
    DAQRunner::cameraSystem.Shutdown();
    return 0;
}