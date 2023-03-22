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
			cv::Mat _tempCVImage = cv::Mat1b(std::get<2>(_tmpMetadata), std::get<3>(_tmpMetadata),
				_tmpImg.data(), (size_t)std::get<3>(_tmpMetadata)*sizeof(VmbUchar_t));
			cv::waitKey(1);
			cv::imshow("FrameConsumer Debug/Test Window", _tempCVImage);
			this->MatMutex.lock();
			this->OpenCVMatQueue.push(_tempCVImage.clone());
			this->OpenCVMetadata.push(_tmpMetadata);
			this->MatMutex.unlock();
		}
	}
	return;
}

void AVFrameConsumer::Encoder(std::string _captureFile)
{
	// Data structure for meta data
	// tuple(frame ID*, timestamp*, imgH, imgW)
	// * = VmbUint64_t, rest are VmbUint32_t
	std::cout << "Encoder worker starting up..." << std::endl;
	VmbUint64_t _oldTimestampNano = 0;
	unsigned int _frameDelta;
	double _currentFPS = 0.0;
	cv::VideoWriter _cvVidWriter;
	bool writerIsInitialized = false;
	std::cout << "ENCODER: Using " << _captureFile
		<< " as  file name." << std::endl;
	auto _fullfilepath = std::string("C:\\ded-test\\") + _captureFile + std::string(".avi");
	while (this->amIListening == true)
	{
		if ((this->OpenCVMatQueue.empty() == false) &&
			(this->OpenCVMetadata.empty() == false))
		{
			this->MatMutex.lock();	// grab mutex and frame + info
			auto _unprocOCVFrame = this->OpenCVMatQueue.front().clone();
			auto _unprocMetadata = this->OpenCVMetadata.front();
			int frameID = std::get<0>(_unprocMetadata);
			int timestamp = std::get<1>(_unprocMetadata);
			int imageH = std::get<2>(_unprocMetadata);
			int imageW = std::get<3>(_unprocMetadata);
			this->OpenCVMatQueue.pop();
			this->OpenCVMetadata.pop();
			this->MatMutex.unlock();
			// Calculate the frame delta
			_frameDelta = timestamp - _oldTimestampNano;
			_oldTimestampNano = timestamp;
			//                1             
			// FPS = --------------------
			//        Delta[ns] / (10^9)    
			_currentFPS = 1 / (_frameDelta / std::pow(10.0, 9.0));
			// Check to see if the videowriter has been initialized
			// need to wait until the first frame is recieved in order
			//.to know the dimensions of the frame.
			if (writerIsInitialized == false)
			{
				_cvVidWriter = cv::VideoWriter(_fullfilepath, cv::CAP_FFMPEG, this->codec, 60.0,
					cv::Size(imageW,imageH), false);
				writerIsInitialized = true;
			}
			else {
				// code to write out the frame goes here....
				_cvVidWriter.write(_unprocOCVFrame);
			}
		} else {
			// naptime
			std::this_thread::sleep_for((this->EmptyQueueSleepTime));
		}
	}
	std::cout << "There are " << this->OpenCVMatQueue.size() << " frames left!" << std::endl;
	std::cout << "Queue empty property is " << this->OpenCVMatQueue.empty() << std::endl;
	if (this->OpenCVMatQueue.empty() == false)
	{
		while (this->OpenCVMatQueue.empty() == false)
		{
			this->MatMutex.lock();
			auto _unprocOCVFrame = this->OpenCVMatQueue.front().clone();
			auto _unprocMetadata = this->OpenCVMetadata.front();
			int frameID = std::get<0>(_unprocMetadata);
			int timestamp = std::get<1>(_unprocMetadata);
			int imageH = std::get<2>(_unprocMetadata);
			int imageW = std::get<3>(_unprocMetadata);
			this->OpenCVMatQueue.pop();
			this->OpenCVMetadata.pop();
			this->MatMutex.unlock();
			// Calculate the frame delta
			_frameDelta = timestamp - _oldTimestampNano;
			_oldTimestampNano = timestamp;
			//                1             
			// FPS = --------------------
			//        Delta[ns] / (10^9)    
			_currentFPS = 1 / (_frameDelta / std::pow(10.0, 9.0));
			_cvVidWriter.write(_unprocOCVFrame);
			std::cout << "There are " << this->OpenCVMatQueue.size() << " frames left to encode!"
				<< std::endl;
		}
	}
	std::cout << "Encoding finished! Shutting down!" << std::endl;
	// Shutdown the videowriter
	if (writerIsInitialized == true)
	{
		_cvVidWriter.release();
	}
}

void AVFrameConsumer::StartConsumer()
{
	this->amIListening = true;
	this->consumerThread = std::thread(&AVFrameConsumer::Consumer, this);
	std::this_thread::sleep_for(250ms);
	this->encoderThread = std::thread([&] { this->Encoder("dummy"); });
	return;
}

void AVFrameConsumer::StopConsumer()
{
	this->amIListening = false;
	this->consumerThread.join();
	this->encoderThread.join();
	std::cout << "Consumer grabbed " << this->framesConsumed << " frames." << std::endl;
	return;
}