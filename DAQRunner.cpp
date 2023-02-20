#include "DAQRunner.h"
#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <condition_variable>
#include "VimbaCPP/Include/VimbaSystem.h"
#include "VimbaCPP/Include/IFrameObserver.h"
#include "AVCameraObject.h"

using namespace AVT::VmbAPI;

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
    CameraPtr _currentCamera;
    std::vector<std::string> _configKeywords = {"ExposureMode", "ExposureTime",
                                "AcquisitionMode"};
    std::vector<std::string> _configValues = {"Timed", "N/A", "Continuous"};

    std::vector<float> _configValuesFloat = { 0.0, 5000.0, 0.0 };
    // For each camera, initialize with some default values
    for (auto sIter = std::begin(DAQRunner::AVCameraIDs); sIter != std::end(DAQRunner::AVCameraIDs);
        sIter++)
    {
        if (VmbErrorSuccess == DAQRunner::cameraSystem.GetCameraByID((*sIter).c_str(),
            _currentCamera))
        {
            std::cout << "Sucessfully got pointer to device id " << (*sIter) << std::endl;
            // Try to open the camera
            if (VmbErrorSuccess != _currentCamera->Open(VmbAccessModeFull))
            {
                std::cout << "Something went wrong trying to open the camera " <<
                    "instance for device ID " << (*sIter) << "." << std::endl;
                return -2;
            }
            // If successful, apply the _configKeywords and _configValues vectors
            // to each camera.
            FeaturePtr _tmpFeat;

                for (auto cfgidx = 0; cfgidx < _configKeywords.size(); cfgidx++)
            {
                std::cout << "Setting feature " << _configKeywords[cfgidx] <<
                    " with value " << _configValues[cfgidx] << " on device ID: "
                    << (*sIter) << std::endl;
                if (VmbErrorSuccess != _currentCamera->GetFeatureByName(
                    _configKeywords[cfgidx].c_str(), _tmpFeat))
                {
                    std::cout << "Something went wrong trying to get feature " << _configKeywords[cfgidx]
                        << " on device id " << (*sIter) << "." << std::endl;
                    return -3;
                }
                if (_configValues[cfgidx] == std::string("N/A"))
                {
                    // This means that the float vector should be used because
                    // I am dumb and don't really know a good way to implement the 
                    // multitype vector.
                    if (VmbErrorSuccess != _tmpFeat->SetValue(_configValuesFloat[cfgidx]))
                    {
                        std::cout << "Something went wrong writing the feature (float) to the camera!" << std::endl;
                        return -4;
                    }
                }
                else {
                    // Set the feature to the new value
                    if (VmbErrorSuccess != _tmpFeat->SetValue(_configValues[cfgidx].c_str()))
                    {
                        std::cout << "Something went wrong writing the feature (std::string) to the camera!" << std::endl;
                        return -4;
                    }
                }
            }
            if (VmbErrorSuccess != _currentCamera->Close())
            {
                std::cout << "Problem closing camera cleanly after configuration." << std::endl;
                return -5;
            }
        }
        else {
            std::cout << "Problem during connection attempt to " <<
                (*sIter) << "." << std::endl;
            return -1;
        }
    }
}

int DAQRunner::setupCapture(int _ptsToCap, float _freqToCap)
{
    // Clear the buffers out
    framebuf1.clear();
    framebuf2.clear();

    return 0;
}

int DAQRunner::startStreaming(std::string _camID)
{
    // Connect to the cameraa 
    return 0;
}
int DAQRunner::shutdownAVSystem()
{
    DAQRunner::cameraSystem.Shutdown();
    return 0;
}