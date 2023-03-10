#include "AVMonoCamera.h"
#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "VimbaCPP/Include/VimbaCPP.h"
#include "VimbaCPP/Include/VimbaSystem.h"
#include "VimbaCPP/Include/Camera.h"
#include "VimbaCPP/Include/Feature.h"
#include "VimbaCPP/Include/SharedPointerDefines.h"
#include "AVCameraConfiguration.h"

/*
    AVMonoCamera
    Thomas Ales | Iowa State Univ.
    Feb 2023
    ---
    Object that represents an Allied Vision Aluvium 
    camera that is connected to the system. Unique by
    CameraID.
*/

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
    
    int idx = 0;
    // Apply each feature, use the idx variable to correlate the featureName
    // with the featureValue.
    for (auto featIter = AVMonoCamera::associatedConfig.featureName.begin();
        featIter != AVMonoCamera::associatedConfig.featureName.end(); featIter++)
    {
        std::cout << "Attempting to apply FeatureName: " << featIter->c_str() << "\tFeatureValue: ";
        auto retcode = AVMonoCamera::monoCameraPtr->GetFeatureByName(
            featIter->c_str(), AVMonoCamera::cameraFeaturePtr);
        // If getting the feature is successful, determine the associated value type and
        // retrieve + apply it to the camera.
        if (retcode == VmbErrorSuccess)
        {
            if (std::holds_alternative<std::string>(AVMonoCamera::associatedConfig.featureValue[idx]))
            {
                auto fVal = std::get<std::string>(AVMonoCamera::associatedConfig.featureValue[idx]).c_str();
                std::cout << fVal << "." << std::endl;
                // Try applying the feature. Return -2 on feature application failure.
                // Copy/Pasted below.
                if (VmbErrorSuccess != AVMonoCamera::cameraFeaturePtr->SetValue(fVal))
                {
                    std::cout << std::endl << "Setting the feature didn't work!" << std::endl;
                    return -2;
                }
            }
            else if (std::holds_alternative<double>(AVMonoCamera::associatedConfig.featureValue[idx]))
            {
                auto fVal = std::get<double>(AVMonoCamera::associatedConfig.featureValue[idx]);
                std::cout << fVal << "." << std::endl;
                if (VmbErrorSuccess != AVMonoCamera::cameraFeaturePtr->SetValue(fVal))
                {
                    std::cout << std::endl << "Setting the feature didn't work!" << std::endl;
                    return -2;
                }
            }
            else if (std::holds_alternative<bool>(AVMonoCamera::associatedConfig.featureValue[idx]))
            {
                auto fVal = std::get<bool>(AVMonoCamera::associatedConfig.featureValue[idx]);
                std::cout << fVal << "." << std::endl;
                if (VmbErrorSuccess != AVMonoCamera::cameraFeaturePtr->SetValue(fVal))
                {
                    std::cout << std::endl << "Setting the feature didn't work!" << std::endl;
                    return -2;
                }
            }

            idx++;
        }
    }
    // return zero on successfully applying the struct
    return 0;
}
void AVMonoCamera::streamWorker()
{
    // Set the streaming flag to true. Acquire the streaming lock.
    AVMonoCamera::isStreaming = true;
    std::unique_lock streamLock(AVMonoCamera::streamMutex);
    
    // Open the camera with full access, configure it and enter streaming mode 
    // until requested to stop. Need to pass in the class via reference so it 
    // can see the isStreaming flag.
    if (VmbErrorSuccess != AVMonoCamera::monoCameraPtr->Open(VmbAccessModeFull))
    {
        std::cout << "Something went wrong getting camera access for the stream worker!" << std::endl;
    }
   
    // Configure the camera for the experiment. Modify the associated struct
    // if the defaults don't work for your particular application.
    AVMonoCamera::applyFeatureChange();
    std::cout << "streamWorker configured " << AVMonoCamera::cameraID <<
        " successfully." << std::endl;
    // Get the payload size.
    AVMonoCamera::monoCameraPtr->GetFeatureByName("PayloadSize",
        AVMonoCamera::cameraFeaturePtr);
    AVMonoCamera::cameraFeaturePtr->GetValue(AVMonoCamera::cameraPayloadSize);
    
    // Allocate memory for the frames
    std::cout << "WORKER: Allocating memory for frames." << std::endl;
    for (FramePtrVector::iterator fIter = AVMonoCamera::cameraFrameBufferVector.begin();
        fIter != AVMonoCamera::cameraFrameBufferVector.end(); fIter++)
    {
        (*fIter).reset(new Frame(AVMonoCamera::cameraPayloadSize));
        (*fIter)->RegisterObserver(IFrameObserverPtr(
            new AVFrameObserver(AVMonoCamera::monoCameraPtr, AVMonoCamera::ImageQueue,
                AVMonoCamera::streamQueueMutex)));
        AVMonoCamera::monoCameraPtr->AnnounceFrame(*fIter);
    }

    // Start capture engine and put the allocated frames 
    // into the queue.
    std::cout << "WORKER: Starting Vimba API and announcing allocated frames." << std::endl;
    AVMonoCamera::monoCameraPtr->StartCapture();
    for (FramePtrVector::iterator fIter = AVMonoCamera::cameraFrameBufferVector.begin();
        fIter != AVMonoCamera::cameraFrameBufferVector.end(); fIter++)
    {
        AVMonoCamera::monoCameraPtr->QueueFrame(*fIter);
    }
    AVMonoCamera::monoCameraPtr->GetFeatureByName("AcquisitionStart",
        AVMonoCamera::cameraFeaturePtr);
    AVMonoCamera::cameraFeaturePtr->RunCommand();
    std::cout << "WORKER: AcquisitionStart command sent, waiting for quit flag." << std::endl;
   
    // Wait for the condition variable to get triggered.
    AVMonoCamera::streamStopCV.wait(streamLock, [&] 
    { return AVMonoCamera::isStreaming == false; });
    std::cout << "Request to shutdown stream recieved." << std::endl;
    
    // Tear down the API and release the memory used for the frames
    AVMonoCamera::monoCameraPtr->GetFeatureByName("AcquisitionStop",
        AVMonoCamera::cameraFeaturePtr);
    AVMonoCamera::cameraFeaturePtr->RunCommand();
    AVMonoCamera::monoCameraPtr->EndCapture();
    AVMonoCamera::monoCameraPtr->FlushQueue();
    AVMonoCamera::monoCameraPtr->RevokeAllFrames();
    for (FramePtrVector::iterator fIter = AVMonoCamera::cameraFrameBufferVector.begin();
        fIter != AVMonoCamera::cameraFrameBufferVector.end(); fIter++)
    {
        (*fIter)->UnregisterObserver();
    }
    AVMonoCamera::monoCameraPtr->Close();
    return;
}