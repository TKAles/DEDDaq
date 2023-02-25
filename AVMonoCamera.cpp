#include "AVMonoCamera.h"
#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>
#include "VimbaCPP/Include/VimbaCPP.h"
#include "VimbaCPP/Include/VimbaSystem.h"
#include "VimbaCPP/Include/Camera.h"
#include "VimbaCPP/Include/Feature.h"
#include "VimbaCPP/Include/SharedPointerDefines.h"
#include "AVCameraConfiguration.h"

using namespace AVT;
using namespace AVT::VmbAPI;

AVMonoCamera::AVMonoCamera()
{
    AVMonoCamera::isStreaming = false;
}

AVMonoCamera::AVMonoCamera(std::string _camID, VimbaSystem& _cSys)
{
    AVMonoCamera::cameraID = _camID;
    AVMonoCamera::isStreaming = false;
    if (VmbErrorSuccess != _cSys.GetCameraByID(AVMonoCamera::cameraID.c_str(), AVMonoCamera::monoCameraPtr))
    {
        std::cout << "Something went wrong initializing an AVMonoCamera object for ID: " <<
            _camID << "!" << std::endl;
    }
}
// changeFeature - Function to change/set feature on the Allied Vision cameras.
//                 Has overloads for float, bool and int.
//                 Returns 0 on success.
//                        -1 on failure to load the feature object with camera data.
//                        -2 if the hardware does not report the correct value when verifying.

int AVMonoCamera::changeFeature(std::string fName, std::string fValue)
{
    
    // Load the current feature into the featureptr for the camera object
    if (VmbErrorSuccess != AVMonoCamera::monoCameraPtr->Open(VmbAccessModeFull))
    {
        std::cout << "There was an error opening the camera with device ID " << AVMonoCamera::cameraID << "." << std::endl;
    }
    if (VmbErrorSuccess != AVMonoCamera::monoCameraPtr->GetFeatureByName(fName.c_str(), AVMonoCamera::cameraFeaturePtr))
    {
        std::cout << "Error getting camera feature" << std::endl;
        return -1;
    }
    AVMonoCamera::cameraFeaturePtr->SetValue(fValue.c_str());
    // Verify that the feature was correctly set on the hardware side.
    if (VmbErrorSuccess == AVMonoCamera::monoCameraPtr->GetFeatureByName(fName.c_str(), AVMonoCamera::cameraFeaturePtr))
    {
        std::string _tmpResult;
        AVMonoCamera::cameraFeaturePtr->GetValue(_tmpResult);
        if (fValue.compare(_tmpResult) != 0)
        {
            std::cout << "Comparison returned False!" << std::endl;
            std::cout << "Comparison value is " << fValue.compare(_tmpResult) << std::endl;
            return -2;
        }
    }
    AVMonoCamera::monoCameraPtr->Close();
    return 0;
}

// changeFeature - Function to change/set feature on the Allied Vision cameras.
//                 float overload
//                 Returns 0 on success.
//                        -1 on failure to load the feature object with camera data.
//                        -2 if the hardware does not report the correct value when verifying.

int AVMonoCamera::changeFeature(std::string fName, double fValue)
{

    // Load the current feature into the featureptr for the camera object
    if (VmbErrorSuccess != AVMonoCamera::monoCameraPtr->Open(VmbAccessModeFull))
    {
        std::cout << "There was an error opening the camera with device ID " << AVMonoCamera::cameraID << "." << std::endl;
    }
    if (VmbErrorSuccess != AVMonoCamera::monoCameraPtr->GetFeatureByName(fName.c_str(), AVMonoCamera::cameraFeaturePtr))
    {
        std::cout << "Error getting camera feature" << std::endl;
        return -1;
    }
    AVMonoCamera::cameraFeaturePtr->SetValue(fValue);
    // Verify that the feature was correctly set on the hardware side.
    if (VmbErrorSuccess == AVMonoCamera::monoCameraPtr->GetFeatureByName(fName.c_str(), AVMonoCamera::cameraFeaturePtr))
    {
        double _tmpResult;
        double _tolerance = 0.1;
        AVMonoCamera::cameraFeaturePtr->GetValue(_tmpResult);
        if (fabs(_tmpResult - fValue) > _tolerance)
        {
            std::cout << "Comparison evaluates outside specific tolerance!" << std::endl;
            std::cout << "HW is: " << _tmpResult << "\tRequested Value: " << fValue << "!" << std::endl;
            return -2;
        }
        else {
            std::cout << "Comparision within spec" << std::endl;
            std::cout << "HW: " << _tmpResult << "\tRequested: " << fValue << "." << std::endl;
        }
    }
    AVMonoCamera::monoCameraPtr->Close();
    return 0;
}

// changeFeature - Function to change/set feature on the Allied Vision cameras.
//                 bool overload.
//                 Returns 0 on success.
//                        -1 on failure to load the feature object with camera data.
//                        -2 if the hardware does not report the correct value when verifying.

int AVMonoCamera::changeFeature(std::string fName, bool fValue)
{
    return 0;
}

// changeFeature - Function to change/set feature on the Allied Vision cameras.
//                 int overload.
//                 Returns 0 on success.
//                        -1 on failure to load the feature object with camera data.
//                        -2 if the hardware does not report the correct value when verifying.

int AVMonoCamera::changeFeature(std::string fName, int fValue)
{
    return 0;
}

int AVMonoCamera::applyFeatureChange()
{

}
void AVMonoCamera::streamWorker()
{
    AVMonoCamera::isStreaming = true;
    std::cout << "Worker started and waiting on streamLock!" << std::endl;
    std::unique_lock streamLock(AVMonoCamera::streamMutex);
    std::cout << "Worker created unique_lock!" << std::endl;
    AVMonoCamera::streamStopCV.wait(streamLock, [&] { return AVMonoCamera::isStreaming == false; });
    std::cout << "Request to configure recieved...configuring" << std::endl;
    
    return;
}