#pragma once
#include "VimbaCPP/Include/IFrameObserver.h"
#include <mutex>
#include <vector>
#include <queue>

using namespace AVT;
using namespace AVT::VmbAPI;

class AVFrameObserver : public IFrameObserver
{
	std::queue<std::vector<VmbUchar_t>>& _outputQueue;

public:
	AVFrameObserver(CameraPtr pCamera, std::queue<std::vector<VmbUchar_t>>& _imgQueue)
		: IFrameObserver(pCamera), _outputQueue(_imgQueue)
		{
		this->m_pCamera = pCamera;
		};

	void FrameReceived(const FramePtr pFrame);
};