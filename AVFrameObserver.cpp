#include "AVFrameObserver.h"
#include "VimbaCPP/Include/Camera.h"
#include "VimbaCPP/Include/Frame.h"
#include "VimbaCPP/Include/IFrameObserver.h"
#include <iostream>

// Constructor. Pass CameraPtr to function of the camera that
// you want this observer to react to
AVFrameObserver::AVFrameObserver(CameraPtr pCamera) : IFrameObserver(pCamera)
{
	// Constructor
}

// FrameRecieved. Callback function run whenever the associated 
// camera sends a new frame. Needs to re-queue the frame back to
// the camera once copying is completed.
void AVFrameObserver::FrameReceived(const FramePtr pFrame)
{
	// Check the status of the incoming frame to ensure
	// it is actually valid.
	VmbFrameStatusType incomingStatus;
	if (VmbErrorSuccess == pFrame->GetReceiveStatus(incomingStatus))
	{
		if (VmbFrameStatusComplete == incomingStatus)
		{
			// Data is good. 
			std::cout << "Got Frame!" << std::endl;
		}
		else {
			// Data is bad.
			std::cout << "Oh No! Bad Frame Data!!" << std::endl;
		}
	}
	else {
		std::cout << "Couldn't get the frame status!" << std::endl;
	}
	// Return the frame back to the queue using
	// the underlying mechanism provided by IFrameObserver
	m_pCamera->QueueFrame(pFrame);
}