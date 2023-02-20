#include "AVCameraObject.h"
#include "AVFrameObserver.h"
#include <string>
#include <iostream>
#include <condition_variable>
#include <mutex>
#include "VimbaCPP/Include/VimbaCPP.h"
#include "VimbaCPP/Include/Camera.h"
#include "VimbaCPP/Include/Feature.h"
#include "VimbaCPP/Include/Frame.h"
#include "VimbaCPP/Include/VimbaSystem.h"

AVCameraObject::AVCameraObject(std::string _camID)
{
    cameraID = _camID;
    
    std::cout << "Created AVCameraObject:" << _camID << std::endl;
}

// Connects to the camera specified in the cameraID field.
// Needs a reference to the Vimba instance.
// Returns 0 if successful
int AVCameraObject::connect(VimbaSystem& _camSys)
{
    AVCameraObject::_associatedCamera = CameraPtr();

    auto result = _camSys.GetCameraByID(
        AVCameraObject::cameraID.c_str(), AVCameraObject::_associatedCamera);
    if (result == VmbErrorSuccess)
    {
        std::cout << "Got pointer to " << AVCameraObject::cameraID << std::endl;
        AVCameraObject::isConnected = true;
        return 0;
        
    }
    else {
        return -1;
    }
}

// configureFeature sets _featureName with the float _featureValue
// needs a reference to the Vimba singleton.
// Returns   0 if sucessful. 
//          -1 if there is an error getting the feature. 
//          -2 if there is an error setting the feature.
//          -3 indicates an access problem. 
//          -4 is an error closing the camera after setting the feature.
int AVCameraObject::configureFeature(std::string _featureName, float _featureValue,
    VimbaSystem& _camSys)
{
    // Get the reference to the vimba driver and use that to grab the camera
    // reference as well.
    if (VmbErrorSuccess == _camSys.GetCameraByID(AVCameraObject::cameraID.c_str(),
        AVCameraObject::_associatedCamera))
    {
        // Camera needs to be opened, then the feature can be set.
        // close camera on sucess
        if (VmbErrorSuccess != AVCameraObject::_associatedCamera->Open(VmbAccessModeFull))
        {
            std::cout << "Error opening camera for access!" << std::endl;
            return -3;
        }
        // If open went ok, get the _associatedFeature with the 
        // feature requested and use the setValue function to write
        // the new value
        if (VmbErrorSuccess == AVCameraObject::_associatedCamera->GetFeatureByName(
            _featureName.c_str(), AVCameraObject::_associatedFeature))
        {
            if (VmbErrorSuccess == AVCameraObject::_associatedFeature->SetValue(_featureValue))
            {
                if (VmbErrorSuccess == AVCameraObject::_associatedCamera->Close())
                {
                    return 0;
                }
                else {
                    std::cout << "Issue closing camera after setting feature!" << std::endl;
                    return -4;
                }
            }
            else {
                std::cout << "Something went wrong setting the feature!" << std::endl;
                return -2;
            }
        }
        else {
            std::cout << "Something went wrong getting the feature!" << std::endl;
            return -1;
        }
    }

}

// configureFeature sets _featureName with the overloaded int _featureValue
// needs a reference to the Vimba singleton.
// Returns   0 if sucessful. 
//          -1 if there is an error getting the feature. 
//          -2 if there is an error setting the feature.
//          -3 indicates an access problem. 
//          -4 is an error closing the camera after setting the feature.
int AVCameraObject::configureFeature(std::string _featureName, int _featureValue,
    VimbaSystem& _camSys)
{
    // Get the reference to the vimba driver and use that to grab the camera
    // reference as well.
    if (VmbErrorSuccess == _camSys.GetCameraByID(AVCameraObject::cameraID.c_str(),
        AVCameraObject::_associatedCamera))
    {
        // Camera needs to be opened, then the feature can be set.
        // close camera on sucess
        if (VmbErrorSuccess != AVCameraObject::_associatedCamera->Open(VmbAccessModeFull))
        {
            std::cout << "Error opening camera for access!" << std::endl;
            return -3;
        }
        // If open went ok, get the _associatedFeature with the 
        // feature requested and use the setValue function to write
        // the new value
        if (VmbErrorSuccess == AVCameraObject::_associatedCamera->GetFeatureByName(
            _featureName.c_str(), AVCameraObject::_associatedFeature))
        {
            if (VmbErrorSuccess == AVCameraObject::_associatedFeature->SetValue(_featureValue))
            {
                if (VmbErrorSuccess == AVCameraObject::_associatedCamera->Close())
                {
                    return 0;
                }
                else {
                    std::cout << "Issue closing camera after setting feature!" << std::endl;
                    return -4;
                }
            }
            else {
                std::cout << "Something went wrong setting the feature!" << std::endl;
                return -2;
            }
        }
        else {
            std::cout << "Something went wrong getting the feature!" << std::endl;
            return -1;
        }
    }

}

// configureFeature sets _featureName with the overloaded std::string _featureValue
// needs a reference to the Vimba singleton.
// Returns   0 if sucessful. 
//          -1 if there is an error getting the feature. 
//          -2 if there is an error setting the feature.
//          -3 indicates an access problem. 
//          -4 is an error closing the camera after setting the feature.
int AVCameraObject::configureFeature(std::string _featureName, std::string _featureValue,
    VimbaSystem& _camSys)
{
    // Get the reference to the vimba driver and use that to grab the camera
    // reference as well.
    if (VmbErrorSuccess == _camSys.GetCameraByID(AVCameraObject::cameraID.c_str(),
        AVCameraObject::_associatedCamera))
    {
        // Camera needs to be opened, then the feature can be set.
        // close camera on sucess
        if (VmbErrorSuccess != AVCameraObject::_associatedCamera->Open(VmbAccessModeFull))
        {
            std::cout << "Error opening camera for access!" << std::endl;
            return -3;
        }
        // If open went ok, get the _associatedFeature with the 
        // feature requested and use the setValue function to write
        // the new value
        if (VmbErrorSuccess == AVCameraObject::_associatedCamera->GetFeatureByName(
            _featureName.c_str(), AVCameraObject::_associatedFeature))
        {
            if (VmbErrorSuccess == AVCameraObject::_associatedFeature->SetValue(_featureValue.c_str()))
            {
                if (VmbErrorSuccess == AVCameraObject::_associatedCamera->Close())
                {
                    return 0;
                } else {
                    std::cout << "Issue closing camera after setting feature!" << std::endl;
                    return -4;
                    }
            }
            else {
                std::cout << "Something went wrong setting the feature!" << std::endl;
                return -2;
            }
        }
        else {
            std::cout << "Something went wrong getting the feature!" << std::endl;
            return -1;
        }
    }

}

// Shuts down the camera object.
// Returns 0 on success, -1 on error.
int AVCameraObject::disconnect()
{
    if (VmbErrorSuccess == AVCameraObject::_associatedCamera->Close())
    {
        AVCameraObject::isConnected = false;
        return 0;
    }
    else {
        std::cout << "Something went wrong destroying device " <<
            cameraID << "!" << std::endl;
        return -1;
    }
}


int AVCameraObject::startStream(VimbaSystem& _camSys)
{
    std::unique_lock _sLock(AVCameraObject::streamLockMutex);
    
    if (VmbErrorSuccess != _camSys.GetCameraByID(AVCameraObject::cameraID.c_str(),
        AVCameraObject::_associatedCamera))
    {
        std::cout << "Error getting the camera pointer to start the stream!" << std::endl;
        return -1;
    }
    
    // If getting the camera pointer was successful, open the camera and pull out
    // the payload size, image width and height 
    if (VmbErrorSuccess != AVCameraObject::_associatedCamera->Open(VmbAccessModeFull))
    {
        std::cout << "Error opening camera with full access. Aborting." << std::endl;
        return -2;
    }
    // Get payload size
    
    FeaturePtr _tmpFeaturePtr;
    if (VmbErrorSuccess == AVCameraObject::_associatedCamera->GetFeatureByName("PayloadSize",
                                                                                _tmpFeaturePtr))
    {
        VmbInt64_t _tmpValue;
        _tmpFeaturePtr->GetValue(_tmpValue);
        std::cout << "Payload size is: " << _tmpValue << " bytes" << std::endl;
        AVCameraObject::payloadSize = _tmpValue;
    }; 
    // Setup stream mode.
}
