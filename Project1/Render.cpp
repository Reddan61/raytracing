#include "Render.h"

Render::Render(int width, int height)
{
	Viewport* viewport = new Viewport(width, height);

	this->viewport = viewport;
}

Render::~Render()
{
	delete this->viewport;
}

const sf::Vector3f& Render::calculateDirection(int x, int y)
{
	auto scale = this->viewport->getSize();

	return sf::Vector3f(x * 1.f / scale.x, y * 1.f / scale.y, 1.f);
}

const sf::Color& Render::traceRay(Scene* scene, sf::Vector3f& cameraPosition, sf::Vector3f& direction, int min_t, int max_t)
{
	float closest_t = Infinity;
	Object* closest_object = nullptr;

	auto objects = scene->getSceneObjects();

	for (int i = 0; i < objects->size(); i++) {
		auto result = objects->at(i)->insertRay(cameraPosition, direction);

		if (result.x < closest_t && min_t < result.x && result.x < max_t) {
			closest_t = result.x;
			closest_object = objects->at(i);
		}
		if (result.y < closest_t && min_t < result.y && result.y < max_t) {
			closest_t = result.y;
			closest_object = objects->at(i);
		}
	}

	if (closest_object == nullptr) {
		return sf::Color(255, 255, 255, 255);
	}

	sf::Vector3f point = cameraPosition + (direction * closest_t);
	sf::Vector3f normal = point - closest_object->getPosition();
	normal = (1.0f / Math::LengthVector(normal)) * normal;

	return Math::Multiply(this->ComputeLighting(scene, point, normal), closest_object->getColor());
}

float Render::ComputeLighting(Scene* scene, sf::Vector3f& point, sf::Vector3f& normal)
{
	float bright = scene->getSceneAmbientLight()->getBright();
	float length_n = Math::LengthVector(normal);

	auto lights = scene->getSceneLights();

	for (int i = 0; i < lights->size(); i++) {
		Light *light = lights->at(i);

		auto lightVector = light->getLightVector(point);

		auto n_dot_l = Math::GetDotProduct(normal, lightVector);

		if (n_dot_l > 0) {
			bright += light->getBright() * n_dot_l / ( length_n * Math::LengthVector(lightVector));
		}
	}

	return bright;
}

void Render::calculate(Scene* scene)
{
	auto scale = this->viewport->getSize();
	auto pixels = this->viewport->getPixels();
	auto cameraPosition = scene->getCamera()->getPosition();

	for (int x = -scale.x / 2; x < scale.x / 2; x++) {
		for (int y = -scale.y / 2; y < scale.y / 2; y++) {
			auto direction = this->calculateDirection(x, y);

			auto color = this->traceRay(scene, cameraPosition, direction, 1, Infinity);

			this->viewport->updatePixel(scale.x / 2 + x, scale.y / 2 - y - 1, color);
		}
	}
}


void Render::update(Scene* scene, sf::RenderWindow& window, sf::Time time)
{
	this->calculate(scene);
	this->viewport->update(window);
}
