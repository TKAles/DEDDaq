#pragma once
#include "VimbaCPP/Include/IFrameObserver.h"
#include <mutex>
#include <queue>

using namespace AVT;
using namespace AVT::VmbAPI;

class AVFrameObserver : public IFrameObserver
{
public:
	AVFrameObserver(CameraPtr pCamera) : IFrameObserver(pCamera) {};
	void FrameReceived(const FramePtr pFrame);
	void GetFrame(FramePtr& _outFrame);
	std::mutex queueMutex;
	std::queue<FramePtr> frameTransferQueue;

};