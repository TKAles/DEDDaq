#pragma once
#include "VimbaCPP/Include/IFrameObserver.h"
#include <mutex>
#include <vector>
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
	std::queue<std::vector<VmbUchar_t>>& _outputQueue;
	std::mutex& _queueMutex;

public:
	AVFrameObserver(CameraPtr pCamera, std::queue<std::vector<VmbUchar_t>>& _imgQueue,
					std::mutex& _qMut)
		: IFrameObserver(pCamera), _outputQueue(_imgQueue), _queueMutex(_qMut) {};

	void FrameReceived(const FramePtr pFrame);
};