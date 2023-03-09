#pragma once
#include <vector>
#include <string>
#include <variant>

/*
	AVCameraConfiguration
	Thomas Ales | Iowa State Univ.
	Feb 2023
	---

	Custom Datatype used to hold settings to configure the 
	Allied Vision monochrome cameras for various experiments.
	Set the TriggerMode to Off to enable free-run at a given exposure
	value.
*/
class AVCameraConfiguration
{
	public:
		std::vector<std::string> featureName;
		std::vector<std::variant<std::string, double, int, bool>> featureValue;
		AVCameraConfiguration()
		{
			// Setting names are from the Allied Vision Features Manual

			featureName.push_back("AcquisitionMode");
			featureName.push_back("ExposureMode");
			featureName.push_back("ExposureTime");
			featureName.push_back("TriggerActivation");
			featureName.push_back("TriggerMode");
			featureName.push_back("TriggerSelector");
			featureName.push_back("TriggerSource");
			featureName.push_back("Gain");
			
			// Values depend on setting, but can be either bool, double, int or std::string
			featureValue.push_back((std::string)"Continuous");
			featureValue.push_back((std::string)"Timed");
			featureValue.push_back((double)5000.0);
			featureValue.push_back((std::string)"RisingEdge");
			featureValue.push_back((std::string)"Off");
			featureValue.push_back((std::string)"ExposureStart");
			featureValue.push_back((std::string)"Line0");
			featureValue.push_back((double)24.0);


		}
};