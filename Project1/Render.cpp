#include "Render.h"

Render::Render(int width, int height)
{
	Viewport* viewport = new Viewport(width, height);

	this->viewport = viewport;
}

Render::Render(Render& other)
{
	std::cout << "copy" << std::endl;
}

Render::~Render()
{
	delete this->viewport;
}

Vector3d Render::calculateDirection(float x, float y)
{
	auto scale = this->viewport->getSize();

	return Vector3d(x * 1.f / scale.x, y * 1.f / scale.y, 1.f);
}

Color Render::traceRay(Scene* scene, Vector3d& cameraPosition, Vector3d& direction, float min_t, float max_t, int reflection_depth, const Object const* origin)
{
	Object::InsertRayValue intersection = this->getClosesetObject(scene, cameraPosition, direction, min_t, max_t, origin);

	if (intersection.object == nullptr) {
		return scene->getSky()->getColor(direction);
	}

	Vector3d point = cameraPosition + (direction * intersection.t1);

	Vector3d normal = intersection.object->getNormal(point, direction);
	Vector3d view = -1.f * direction;

	float lighting = this->ComputeLighting(scene, point, normal, view, intersection.object->getSpecular(), intersection.object);
	Color local_color = lighting * intersection.object->getColor();

	if (intersection.object->getReflective() <= 0 || reflection_depth <= 0) {
		return local_color;
	}

	Vector3d reflected_ray = this->getReflectRay(view, normal);
	Color reflected_color = this->traceRay(scene, point, reflected_ray, Epsilon, Infinity, reflection_depth - 1, intersection.object);

	float reflective = intersection.object->getReflective();
	
	return ((1.f - reflective) * local_color) + (reflective * reflected_color);
}

float Render::ComputeLighting(Scene* scene, Vector3d& point, Vector3d& normal, Vector3d& view, float specular, const Object const* origin)
{
	float bright = 0;
	float length_n = Math::LengthVector(normal);
	float length_v = Math::LengthVector(view);

	auto lights = scene->getSceneLights();

	for (int i = 0; i < lights->size(); i++) {
		Light *light = lights->at(i);

		// Ambient light
		if (!light->hasPosition()) {
			bright += light->getBright();
			
			continue;
		}

		Vector3d lightVector = light->getLightVector(point);
		float max_t = light->getMaxT();

		// Shadow check.
		Object::InsertRayValue shadow_blocker = this->getClosesetObject(scene, point, lightVector, Epsilon, max_t, origin);

		if (shadow_blocker.object != nullptr) {
			continue;
		}

		// Diffuse reflection
		float n_dot_l = Math::GetDotProduct(normal, lightVector);

		if (n_dot_l > 0) {
			bright += light->getBright() * n_dot_l / ( length_n * Math::LengthVector(lightVector));
		}

		// Specular reflection.
		if (specular != -1) {
			Vector3d vec_r = this->getReflectRay(lightVector, normal);
			float r_dot_v = Math::GetDotProduct(vec_r, view);

			if (r_dot_v > 0) {
				bright += light->getBright() * std::pow(r_dot_v / (Math::LengthVector(vec_r) * length_v), specular);
			}
		}
	}

	return bright;
}

Object::InsertRayValue Render::getClosesetObject(Scene* scene, Vector3d& cameraPosition, Vector3d& direction, float min_t, float max_t, const Object const* origin)
{
	Object::InsertRayValue intersection(Infinity, Infinity, nullptr);

	auto objects = scene->getSceneObjects();

	for (int i = 0; i < objects->size(); i++) {
		Object *object = objects->at(i).get();

		if (origin == object) continue;

		Object::InsertRayValue result = object->insertRay(cameraPosition, direction);

		if (result.t1 < intersection.t1 && min_t < result.t1 && result.t1 < max_t) {
			intersection = result;
			intersection.t2 = result.t1;
		}
		if (result.t2 < intersection.t2 && min_t < result.t2 && result.t2 < max_t) {
			intersection = result;
			intersection.t1 = result.t2;
		}
	}

	return intersection;
}

Vector3d Render::getReflectRay(const Vector3d& const v1, const Vector3d& const v2)
{
	return 2.f * v2 * Math::GetDotProduct(v2, v1) - v1;
}

Color Render::getPixelColor(float x, float y, Scene* scene, Matrix4d& rotation, Vector3d& cameraPosition) {
	Vector3d direction = this->calculateDirection(x, y);

	direction.x = direction.x * 2.0f - 1.0f;
	direction.y = 1.0f - 2.0f * direction.y;

	direction = rotation * direction;

	direction = direction.normalize();

	Color color = this->traceRay(scene, cameraPosition, direction, 1, Infinity, this->reflection_depth);

	return color;
}

Color Render::perPixel(float x, float y, Scene* scene, Matrix4d& rotation, Vector3d& cameraPosition)
{
	// anti-aliasing
	float aa = 4.f;
	float half_aa = aa / 2;
	float partial_aa = 1.f / half_aa;

	Vector3d color;

	float pixel_start_x = x - 0.5f;
	float pixel_start_y = y - 0.5f;

	for (int i = 0; i < half_aa; i++) {
		float u = pixel_start_x + (partial_aa * i);
		float v = pixel_start_y + (partial_aa * i);

		color = color + this->getPixelColor(u, v, scene, rotation, cameraPosition);
	}

	pixel_start_x = x + 0.5f;

	for (int i = 0; i < half_aa; i++) {
		float u = pixel_start_x - (partial_aa * i);
		float v = pixel_start_y + (partial_aa * i);

		color = color + this->getPixelColor(u, v, scene, rotation, cameraPosition);
	}

	Vector3d result_ns = color / aa;

	this->viewport->updatePixel(x, y, Color(result_ns));

	return Color(result_ns);
}

void Render::calculate(Scene* scene, ThreadPool& pool, GPU&)
{
	auto scale = this->viewport->getSize();
	auto cameraPosition = scene->getCamera()->getPosition();
	Matrix4d rotation = scene->getCamera()->getRotation();

	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	std::vector<cl::Device> devices;
	platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);
	//std::cout << devices[0].getInfo<CL_DEVICE_NAME>();

	cl::Device gpu = devices[0];
	cl::Context context(gpu);
	cl::CommandQueue gpu_queue(context, gpu);

	std::ifstream sourceFile("./RayTracing.cl");
	std::string sourceCode(
		std::istreambuf_iterator<char>(sourceFile),
		(std::istreambuf_iterator<char>()));

	cl::Program::Sources sources;
	sources.push_back({ sourceCode.c_str(), sourceCode.length() });

	cl::Program program(context, sources);
	int ret = program.build(gpu, "-I ./C99/");
	//int ret =  program.build(gpu, "-I P:/libs/SFML/SFML-2.6.1/include");
	std::cout << ret << std::endl;
	std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(gpu) << std::endl;
	std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_OPTIONS>(gpu) << std::endl;
	std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(gpu) << std::endl;
	cl::Kernel kernel(program, "GetPerPixel");

	//auto kernel = gpu.getKernel("GetPerPixel");
	//auto context = gpu.getContext();
	//auto gpu_queue = gpu.getQueue();

	//Color pixels[800][1200];
	//Pixel ***pixels = this->viewport->getPixels();
	//Color* pixels = new Color[scale.y * scale.x];
	//float size = scale.y * scale.x * sizeof(Color);

	float* pixels = new float[scale.y * scale.x];
	float size = scale.y * scale.x * sizeof(float);

	//cl::Buffer renderBuff = cl::Buffer(context, CL_MEM_READ_ONLY, sizeof(this), this);
	//cl::Buffer sceneBuff = cl::Buffer(context, CL_MEM_READ_ONLY, sizeof(scene), scene);
	//cl::Buffer rotationBuff = cl::Buffer(context, CL_MEM_READ_ONLY, sizeof(rotation), &rotation);
	//cl::Buffer cameraPosBuff = cl::Buffer(context, CL_MEM_READ_ONLY , sizeof(cameraPosition), &cameraPosition);

	cl::Buffer pixelsBuff = cl::Buffer(
		context, 
		CL_MEM_WRITE_ONLY, 
		size
	);

	int threads_count = pool.getThreadsCount();

	int sizeY = scale.y / threads_count;

	//for (int i = 0; i < threads_count; i++) {
	//	pool.addTask([i, sizeY, &scale,
	//		scene, &rotation, &cameraPosition, 
	//		//&kernel, 
	//		//&renderBuff, 
	//		//&sceneBuff, &rotationBuff, &cameraPosBuff, 
	//		//&gpu_queue,
	//		//&pixelsBuff,
	//		this ]() {
	//		int height = sizeY * (i + 1);

	//		for (int y = sizeY * i; y < height; y++) {
	//			for (int x = 0; x < scale.x; x++) {
	//		/*		kernel.setArg(0, renderBuff);
	//				kernel.setArg(1, sceneBuff);
	//				kernel.setArg(2, rotationBuff);
	//				kernel.setArg(3, cameraPosBuff);
	//				kernel.setArg(4, x);
	//				kernel.setArg(5, y);*/
	//				//kernel.setArg(6, pixelsBuff);
	//				//kernel.setArg(0, pixelsBuff);

	//		/*		gpu_queue.enqueueNDRangeKernel(
	//					kernel, 
	//					cl::NullRange, 
	//					cl::NDRange(scale.x * scale.y), 
	//					cl::NDRange(64)
	//				);*/
	//				this->perPixel(x, y, scene, rotation, cameraPosition);
	//			}
	//		}
	//	});
	//}

	//kernel.setArg(0, renderBuff);
	//kernel.setArg(1, sceneBuff);
	//kernel.setArg(2, rotationBuff);
	//kernel.setArg(3, cameraPosBuff);
	//kernel.setArg(4, 0);
	//kernel.setArg(5, 0);
	kernel.setArg(0, pixelsBuff);

	gpu_queue.enqueueNDRangeKernel(
		kernel,
		cl::NullRange,
		cl::NDRange(scale.x * scale.y),
		cl::NDRange(64)
	);

	/*int totalColoredPixels = sizeY * threads_count;

	if (totalColoredPixels != scale.y) {
		pool.addTask([totalColoredPixels, &scale, scene, &rotation, &cameraPosition, this]() {
			for (int y = totalColoredPixels; y < scale.y; y++) {
				for (int x = 0; x < scale.x; x++) {
					this->perPixel(x, y, scene, rotation, cameraPosition);
				}
			}
		});
	}*/

	pool.wait();
	gpu_queue.finish();

	gpu_queue.enqueueReadBuffer(pixelsBuff, CL_TRUE, 0, size, pixels);

	std::cout << "result = " << pixels[0] << std::endl;
	std::cout << "finish" << std::endl;
}


void Render::update(Scene* scene, sf::RenderWindow& window, sf::Time& time, ThreadPool& pool, GPU& gpu)
{
	scene->update(window, time);
	this->calculate(scene, pool, gpu);
	this->viewport->update(window);
}


