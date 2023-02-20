#include <iostream>
#include "AVFrameObserver.h"
#include "VimbaCPP/Include/IFrameObserver.h"
#include "VimbaCPP/Include/Frame.h"
#include "VimbaCPP/Include/Camera.h"

using namespace AVT::VmbAPI;

AVFrameObserver::AVFrameObserver(CameraPtr pCamera)
	: IFrameObserver(pCamera)
{
	// Constructor
}

void AVFrameObserver::FrameReceived(const FramePtr pFrame)
{
	VmbUint64_t _frameID;
	pFrame->GetFrameID(_frameID);
	// Just return the frame for now.
	std::cout << "Returned Frame " << 
		_frameID << std::endl;
	m_pCamera->QueueFrame(pFrame);
}

AVFrameObserver::~AVFrameObserver()
{
	// do something?
}