#pragma once
#include <opencl.hpp>
#include <string>
#include <fstream>

class GPU
{
public:
	GPU();
	~GPU();

	cl::Kernel getKernel(std::string name);
	cl::Context getContext();
	cl::CommandQueue getQueue();
private:
	cl::Program program;
	//cl::Kernel kernel;
	cl::CommandQueue gpu_queue;
	cl::Context context;
};

