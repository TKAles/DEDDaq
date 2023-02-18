#include "AVCameraObject.h"
#include <string>
#include <iostream>
#include "VimbaCPP/Include/VimbaCPP.h"
#include "VimbaCPP/Include/Camera.h"
#include "VimbaCPP/Include/Feature.h"
#include "VimbaCPP/Include/Frame.h"
#include "VimbaCPP/Include/VimbaSystem.h"

AVCameraObject::AVCameraObject(std::string _camID)
{
    cameraID = _camID;
}

int AVCameraObject::connect()
{
    // TODO: Add your implementation code here.
    return 0;
}


int AVCameraObject::disconnect()
{
    // TODO: Add your implementation code here.
    return 0;
}


void AVCameraObject::startStream()
{
    // TODO: Add your implementation code here.
}
