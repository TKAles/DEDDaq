#pragma once
#include <mutex>
#include <chrono>
#include <thread>
class MonoCamProducer
{
public:
	int times_ran;
	bool count_complete;
	void increment_run();

};