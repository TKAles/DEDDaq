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
    // ugh, this really should be a struct. but...effort
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
    // put trigger configuration and such here.
    return 0;
}

// startStreaming should be ran as it's own thread. Trip over the stopStreamingCV
// condition variable to end the acquisition thread for each instance that is
// created. This is only for the monochrome subsystem and most likely will get
// renamed in the near future.
int DAQRunner::startStreaming(std::string _camID)
{
    // unique_lock is so we can use a condition variable to 
    // wait for a signal to stop streaming
    std::unique_lock<std::mutex> _streamWorkerLock(DAQRunner::stopStreamingMutex);
    CameraPtr _monoCam;
    FeaturePtr _monoFeatures;
    // Get the camera
    if (VmbErrorSuccess != DAQRunner::cameraSystem.GetCameraByID(_camID.c_str(), _monoCam))
    {
        std::cout << "Something went wrong starting the streaming interface" << std::endl;
        return -1;
    }
    // initialize the frameptrvector for the framebuffer with 25 frames.
    DAQRunner::_frameBuffer1.reserve(25);

    // lambda needs local scope to 'see' the _killStream variable
    // so pass everything in by reference `[&]`
    DAQRunner::stopStreamingCV.wait(_streamWorkerLock,
                                [&] {
                                        return DAQRunner::_killStream == true;
                                    });

    // acquire ze payload value
    if (VmbErrorSuccess != _monoCam->GetFeatureByName("payloadSize", _monoFeatures))
    {
        std::cout << "Something went wrong trying to get the payload size of the stream!"
            << std::endl;
        return -2;
    }
    // do some wierd registration voodoo I don't understand and am directly
    // ripping from the C++ API examples. This can't end badly at all....
    // Get an iterator and initialize each entry in the frameptrvector with a blank frame
    for (FramePtrVector::iterator pIter = DAQRunner::_frameBuffer1.begin();
        pIter != DAQRunner::_frameBuffer1.end(); pIter++)
    {
        // Clean? the vector and construct new frames using the _payloadSize
        (*pIter).reset(new Frame(DAQRunner::_payloadSize));
        // Register the modified observer as a pointer? for each frame?
        (*pIter)->RegisterObserver(IFrameObserverPtr(new AVFrameObserver(_monoCam)));
        // Announce that the frames are available to the API
        _monoCam->AnnounceFrame(*pIter);
        // Start the capture engine and queue the frames
        _monoCam->StartCapture();
        for (FramePtrVector::iterator fPtrIter = DAQRunner::_frameBuffer1.begin();
            fPtrIter != DAQRunner::_frameBuffer1.end(); fPtrIter++)
        {
            _monoCam->QueueFrame(*fPtrIter);
        }
        // Start acquiring frames.
        _monoCam->GetFeatureByName("AcquisitionStart", _monoFeatures);
        _monoFeatures->RunCommand();
        // Wait for the lock to clear before stopping the acqusition engine.
        DAQRunner::stopStreamingCV.wait(_streamWorkerLock);
        // Stop frame acquistion
        _monoCam->GetFeatureByName("AcquisitionStop", _monoFeatures);
        _monoFeatures->RunCommand();
    }
    return 0;
}
int DAQRunner::shutdownAVSystem()
{
    DAQRunner::cameraSystem.Shutdown();
    return 0;
}