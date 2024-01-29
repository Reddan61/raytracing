#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Scene.h"
#include "Camera.h"
#include "Render.h"
#include "./Objects/Sphere/Sphere.h"
#include "./Objects/Object.h"
#include "./Objects/Triangle/Triangle.h"
#include "Light/AmbientLight.h"
#include "Light/DirectionalLight.h"
#include "Light/PointLight.h"
#include "ThreadPool/ThreadPool.h"
#include "Utils.h"
#include <opencl.hpp>
#include "./GPU/GPU.h"
#include "./Utils/Color/Color.h"

//const int DATA_SIZE = 20 * 1024 * 1024;
//const int TESTS_NUMBER = 5000;
//float* pInputVector1;
//float* pInputVector2;
//float* pOutputVector;
//float* pOutputVectorHost;

void gpuTest() {
	//std::vector<cl::Platform> platforms;
	//cl::Platform::get(&platforms);

	//std::vector<cl::Device> devices;
	//platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);
	////std::cout << devices[0].getInfo<CL_DEVICE_NAME>();

	//cl::Device gpu = devices[0];
	//cl::Context context(gpu);
	//cl::CommandQueue gpu_queue(context, gpu);

	//cl::Buffer clmInputVector1 = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, DATA_SIZE * sizeof(float), pInputVector1);
	//cl::Buffer clmInputVector2 = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, DATA_SIZE * sizeof(float), pInputVector2);
	//cl::Buffer clmOutputVector = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, DATA_SIZE * sizeof(float), pOutputVector);

	//std::ifstream sourceFile("Kernel.cl");
	//std::string sourceCode(
	//	std::istreambuf_iterator<char>(sourceFile),
	//	(std::istreambuf_iterator<char>()));

	//cl::Program::Sources sources;
	//sources.push_back({ sourceCode.c_str(), sourceCode.length() });

	//cl::Program program(context, sources);

	//program.build(gpu);
	//cl::Kernel kernel(program, "TestKernel");

	//int iArg = 0;
	//kernel.setArg(iArg++, clmInputVector1);
	//kernel.setArg(iArg++, clmInputVector2);
	//kernel.setArg(iArg++, clmOutputVector);
	//kernel.setArg(iArg++, DATA_SIZE);

	//gpu_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(DATA_SIZE), cl::NDRange(128));
	//gpu_queue.finish();
	//gpu_queue.enqueueReadBuffer(clmOutputVector, CL_TRUE, 0, DATA_SIZE * sizeof(float), pOutputVector);
}

int main()
{
	int width = 1200;
	int height = 800;

	sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!");

	ThreadPool pool(12);
	GPU gpu;

	auto* objects = new std::vector<Object*>();
	auto* lights = new std::vector<Light*>();

	TriangleMesh* cube = Utils::LoadCustomFormatFile("cube.txt");
	cube->changePosition(Vector3d(2, 1, 0));

	Scene* scene = new Scene(new Camera(0, 0, 0));

	scene->addSphere(new Sphere(Vector3d(-1, 1, 0), Color(0, 255, 0, 255)));
	scene->addSphere(new Sphere(Vector3d(2, 4, 0), Color(0, 255, 0, 255)));
	scene->addSphere(new Sphere(Vector3d(2, 0, -3), Color(0, 255, 0, 255)));
	scene->addSphere(new Sphere(
		Vector3d(0, -5001, 0),
		Color(255, 255, 0, 255),
		5000,
		1000,
		0.5
	));

	scene->addTriangle(new Triangle(Vector3d(0, 0, -3), Vector3d(1, 1, -3), Vector3d(2, 0, -3), Color(0, 0, 255, 255)));
	scene->addTriangle(new Triangle(Vector3d(1, 0, 1), Vector3d(2, 1, 1), Vector3d(3, 0, 1), Color(0, 0, 255, 255)));
	scene->addTriangle(new Triangle(Vector3d(1, 0, -1), Vector3d(2, 1, -1), Vector3d(3, 0, -1), Color(0, 0, 255, 255)));

	scene->addMesh(cube);

	scene->addLight(new AmbientLight(0.2f));
	scene->addLight(new PointLight(Vector3d(3, 4, -3), 0.8f));

	//lights->push_back(new PointLight(Vector3d(2, 1, 0), 0.6f));
	//lights->push_back(new DirectionalLight(Vector3d(1, 4, 4), 0.2f));

	scene->getGpuScene();

	sf::Clock clock;
	Render *render = new Render(width, height);

	while (window.isOpen())
	{
		sf::Event event;
		sf::Time time = clock.restart();

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		render->update(scene, window, time, pool, gpu);

		window.display();
		
		std::cout << "fps: " << 1.f / clock.getElapsedTime().asSeconds() << std::endl;
	}

	return 0;
}