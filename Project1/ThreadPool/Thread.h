#pragma once
#include <thread>

class Thread
{
public:
	int id;
	bool isWorking = true;
	std::thread thread;
};

