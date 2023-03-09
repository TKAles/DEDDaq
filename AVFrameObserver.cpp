#include "AVFrameObserver.h"
#include "VimbaCPP/Include/Camera.h"
#include "VimbaCPP/Include/Frame.h"
#include "VimbaCPP/Include/IFrameObserver.h"
#include "VimbaImageTransform/Include/VmbTransform.h"
#include <vector>
#include <iostream>

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
			VmbUint64_t frameid;
			VmbUint64_t tstamp;
			pFrame->GetFrameID(frameid);
			VmbUint32_t bufsize;
			VmbUint32_t imgsize;
			VmbUint32_t imgH, imgW;
			pFrame->GetBufferSize(bufsize);
			pFrame->GetImageSize(imgsize);
			pFrame->GetTimestamp(tstamp);
			pFrame->GetHeight(imgH);
			pFrame->GetWidth(imgW);
			VmbUchar_t* imgBuf;
			pFrame->GetImage(imgBuf);
			std::vector<VmbUchar_t> newImgBuf;
			newImgBuf.reserve(imgsize);
			newImgBuf.insert(newImgBuf.end(), &imgBuf[0], &imgBuf[imgsize]);
			AVFrameObserver::_outputQueue.push(newImgBuf);
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