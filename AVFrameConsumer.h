#pragma once
#include <queue>
#include <iostream>
#include <vector>
#include <mutex>
#include <tuple>
#include "VimbaCPP/Include/VimbaCPP.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

using namespace std::chrono_literals;

class AVFrameConsumer
{
	public:
		AVFrameConsumer() : inputImageQueue(*(new std::queue<std::vector<VmbUchar_t>>)),
			imageMutex(*(new std::mutex)), inputMetadataQueue(*(new std::queue<std::tuple<
				VmbUint64_t, VmbUint64_t, VmbUint32_t, VmbUint32_t>>)) {};

		AVFrameConsumer(std::queue<std::vector<VmbUchar_t>>& _imQ, std::mutex& _imMut,
			std::queue<std::tuple<VmbUint64_t, VmbUint64_t, VmbUint32_t, VmbUint32_t>>& _imMDQ) :
			inputImageQueue(_imQ), imageMutex(_imMut), inputMetadataQueue(_imMDQ) {};

		void Consumer();
		void StartConsumer();
		void StopConsumer();

	private:
		std::queue<std::vector<VmbUchar_t>>& inputImageQueue;
		std::mutex& imageMutex;
		std::queue<std::tuple<VmbUint64_t, VmbUint64_t,
			VmbUint32_t, VmbUint32_t>>& inputMetadataQueue;
		bool amIListening;
		std::thread consumerThread;
		unsigned int framesConsumed;
};

