#include "AVFrameObserver.h"
#include "VimbaCPP/Include/Camera.h"
#include "VimbaCPP/Include/Frame.h"
#include "VimbaCPP/Include/IFrameObserver.h"
#include <iostream>
/*
	AVFrameObserver
	Thomas Ales | Iowa State Univ.
	Feb 2023
	---
	Implementation of the Allied Vision IFrameObserver
	with a secondary internal queue for frames to be offloaded
	from the camera to.
*/
// Constructor is defined in the header file.

using namespace AVT;
using namespace AVT::VmbAPI;

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
		VmbUint64_t _frameTStamp;

		if (VmbFrameStatusComplete == incomingStatus)
		{
			// Data is good. Create a lock and place the frame into the 
			// processing/output queue.
			std::unique_lock _queueLock(AVFrameObserver::queueMutex);
			AVFrameObserver::frameTransferQueue.push(pFrame);

		}
		else {
			// Data is bad.
			std::cout << "Oh No! Bad Frame Data!!" << std::endl;
		}
	}
	else {
		// Another bad scenario.
		std::cout << "Couldn't get the frame status!" << std::endl;
	}
	// Return the frame back to the queue using
	// the underlying mechanism provided by IFrameObserver
	m_pCamera->QueueFrame(pFrame);
}

void AVFrameObserver::GetFrame(FramePtr &_outFrame)
{
	// Lock the mutex and pop the frame off the top.
	std::unique_lock _queueLock(AVFrameObserver::queueMutex);
	AVFrameObserver::frameTransferQueue.front();
	
}