#pragma once
#include "VimbaCPP/Include/IFrameObserver.h"
#include <mutex>
#include <queue>
/*
	AVFrameObserver
	Thomas Ales | Iowa State Univ.
	Feb 2023
	---
	Implementation of the Allied Vision IFrameObserver
	with a secondary internal queue for frames to be offloaded
	from the camera to.
*/
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