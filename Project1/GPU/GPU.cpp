#include "GPU.h"

GPU::GPU()
{
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	std::vector<cl::Device> devices;
	platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);
	//std::cout << devices[0].getInfo<CL_DEVICE_NAME>();

	cl::Device gpu = devices[0];
	cl::Context context(gpu);
	cl::CommandQueue gpu_queue(context, gpu);

	std::ifstream sourceFile("RayTracing.cl");
	std::string sourceCode(
		std::istreambuf_iterator<char>(sourceFile),
		(std::istreambuf_iterator<char>()));

	cl::Program::Sources sources;
	sources.push_back({ sourceCode.c_str(), sourceCode.length() });

	cl::Program program(context, sources);

	program.build(gpu);

	this->program = program;
	this->gpu_queue = gpu_queue;
	this->context = context;
	//cl::Kernel kernel(this->program, "GetPixelColor");
}

GPU::~GPU()
{
}

cl::Kernel GPU::getKernel(std::string name)
{
	return cl::Kernel(this->program, name.c_str());
}

cl::Context GPU::getContext()
{
	return this->context;
}

cl::CommandQueue GPU::getQueue()
{
	return this->gpu_queue;
}
