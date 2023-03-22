#include "AVFrameObserver.h"
#include "VimbaCPP/Include/Camera.h"
#include "VimbaCPP/Include/Frame.h"
#include "VimbaCPP/Include/IFrameObserver.h"
#include "VimbaImageTransform/Include/VmbTransform.h"
#include <vector>
#include <iostream>
#include <tuple>
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
		if (VmbFrameStatusComplete == incomingStatus)
		{
			// Data is good. Create a lock and place the frame into the 
			// processing/output queue.
			VmbUint32_t bufsize;
			VmbUint32_t imgsize;
			pFrame->GetBufferSize(bufsize);
			pFrame->GetImageSize(imgsize);
			VmbUint32_t imgH;
			VmbUint32_t imgW;
			VmbUint64_t frameID;
			VmbUint64_t timestamp;
			VmbUchar_t* imgBuf;
			pFrame->GetImage(imgBuf);
			pFrame->GetHeight(imgH);
			pFrame->GetWidth(imgW);
			pFrame->GetFrameID(frameID);
			pFrame->GetTimestamp(timestamp);

			std::vector<VmbUchar_t> newImgBuf;
			auto _metadata = std::tuple<VmbUint64_t, VmbUint64_t, VmbUint32_t, VmbUint32_t>(
				frameID, timestamp, imgH, imgW);
			newImgBuf.reserve(imgsize);
			newImgBuf.insert(newImgBuf.end(), &imgBuf[0], &imgBuf[imgsize]);
			AVFrameObserver::_queueMutex.lock();
			AVFrameObserver::_outputQueue.push(newImgBuf);
			AVFrameObserver::_metadataQueue.push(_metadata);
			AVFrameObserver::_queueMutex.unlock();
		} else {
			// Data is bad.
			std::cout << "Oh No! Bad Frame Data!!" << std::endl;
		}
	} else {
		// Some other bad scenario.
		std::cout << "Couldn't get the frame status!" << std::endl;
	}
	// Return the frame back to the queue using
	// the underlying mechanism provided by IFrameObserver
	m_pCamera->QueueFrame(pFrame);
}