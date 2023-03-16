#include "AVFrameConsumer.h"

void AVFrameConsumer::Consumer()
{
	this->framesConsumed = 0;
	
	while (this->amIListening == true)
	{
		if (this->inputImageQueue.empty() == true)
		{
			std::cout << "Queue is empty!" << std::endl;
			std::this_thread::sleep_for(1s);
		}
		else
		{
			// Grab the lock and remove the image frame along
			// with the metadata tuple
			this->imageMutex.lock();
			auto _tmpImg = this->inputImageQueue.front();
			auto _tmpMetadata = this->inputMetadataQueue.front();
			this->inputImageQueue.pop();
			this->inputMetadataQueue.pop();
			this->imageMutex.unlock();
			this->framesConsumed++;
			std::cout << "Metadata: FID: " << std::get<0>(_tmpMetadata) <<
				" Time: " << std::get<1>(_tmpMetadata) << std::endl;
			cv::Mat _tempCVImage = cv::Mat1b(std::get<2>(_tmpMetadata), std::get<3>(_tmpMetadata),
				_tmpImg.data(), (size_t)std::get<3>(_tmpMetadata)*sizeof(VmbUchar_t));
			cv::imshow("FrameConsumer Debug/Test Window", _tempCVImage);
			cv::waitKey(1);
		}
	}
	return;
}

void AVFrameConsumer::StartConsumer()
{
	this->amIListening = true;
	this->consumerThread = std::thread([&] {this->Consumer(); });
	return;
}

void AVFrameConsumer::StopConsumer()
{
	this->amIListening = false;
	this->consumerThread.join();
	std::cout << "Consumer grabbed " << this->framesConsumed << " frames." << std::endl;
	return;
}