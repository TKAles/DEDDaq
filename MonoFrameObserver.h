#pragma once
#include <mutex>
#include <queue>
#include "VimbaCPP/Include/IFrameObserver.h"
#include "VimbaCPP/Include/VimbaCPP.h"

using namespace AVT;
using namespace AVT::VmbAPI;

class MonoFrameObserver : public IFrameObserver
{

public:
	~MonoFrameObserver();
	std::string cameraID;
	void frameRecieved(const AVT::VmbAPI::FramePtr _inFrame);
};

void MonoFrameObserver::frameRecieved(const AVT::VmbAPI::FramePtr _inFrame)
{
	VmbUint64_t _timestamp;
	_inFrame->GetTimestamp(_timestamp);
	std::cout << "Got frame from " << cameraID << " with timestamp of "
		<< _timestamp << "ns." << std::endl;
	IFrameObserver::m_pCamera->QueueFrame(_inFrame);
}

