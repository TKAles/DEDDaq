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
	std::queue<std::tuple<VmbUint64_t, VmbUint64_t, VmbUint32_t, VmbUint32_t>>& _metadataQueue;
	std::mutex& _queueMutex;

public:
	AVFrameObserver(CameraPtr pCamera, std::queue<std::vector<VmbUchar_t>>& _imgQueue,
					std::mutex& _qMut, std::queue<std::tuple<
					VmbUint64_t, VmbUint64_t, VmbUint32_t, VmbUint32_t>>& _MetaQueue)
		: IFrameObserver(pCamera), _outputQueue(_imgQueue), _queueMutex(_qMut),
			_metadataQueue(_MetaQueue) {};

	void FrameReceived(const FramePtr pFrame);
};