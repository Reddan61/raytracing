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

sf::Vector3f Render::calculateDirection(int x, int y)
{
	auto scale = this->viewport->getSize();

	return sf::Vector3f(x * 1.f / scale.x, y * 1.f / scale.y, 1.f);
}

sf::Color Render::traceRay(Scene* scene, sf::Vector3f& cameraPosition, sf::Vector3f& direction, float min_t, float max_t, int reflection_depth, Object* origin)
{
	auto closest = this->getClosesetObject(scene, cameraPosition, direction, min_t, max_t, origin);

	if (closest.first == nullptr) {
		return sf::Color(0, 0, 0, 255);
	}

	sf::Vector3f point = cameraPosition + (direction * closest.second);
	sf::Vector3f normal = point - closest.first->getPosition();
	normal = normal / Math::LengthVector(normal);

	sf::Vector3f view = -1.f * direction;

	float lighting = this->ComputeLighting(scene, point, normal, view, closest.first->getSpecular());
	sf::Color local_color = Math::Multiply(lighting, closest.first->getColor());

	if (closest.first->getReflective() <= 0 || reflection_depth <= 0) {
		return local_color;
	}

	sf::Vector3f reflected_ray = this->getReflectRay(view, normal);
	sf::Color reflected_color = this->traceRay(scene, point, reflected_ray, 0.001f, Infinity, reflection_depth - 1, closest.first);

	float reflective = closest.first->getReflective();
	
	return Math::Add(Math::Multiply(1.f - reflective, local_color), Math::Multiply(reflective, reflected_color));
}

float Render::ComputeLighting(Scene* scene, sf::Vector3f& point, sf::Vector3f& normal, sf::Vector3f& view, float specular)
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

		sf::Vector3f lightVector = light->getLightVector(point);
		float max_t = light->getMaxT();

		// Shadow check.
		auto shadow_blocker = this->getClosesetObject(scene, point, lightVector, 0.001f, max_t);

		if (shadow_blocker.first != nullptr) {
			continue;
		}

		// Diffuse reflection
		float n_dot_l = Math::GetDotProduct(normal, lightVector);

		if (n_dot_l > 0) {
			bright += light->getBright() * n_dot_l / ( length_n * Math::LengthVector(lightVector));
		}

		// Specular reflection.
		if (specular != -1) {
			sf::Vector3f vec_r = this->getReflectRay(lightVector, normal);
			float r_dot_v = Math::GetDotProduct(vec_r, view);

			if (r_dot_v > 0) {
				bright += light->getBright() * std::pow(r_dot_v / (Math::LengthVector(vec_r) * length_v), specular);
			}
		}
	}

	return bright;
}

// first - closest_object
// second - closest_t
// origin - чтобы отражение не падало на свой же обьект
std::pair<Object*, float> Render::getClosesetObject(Scene* scene, sf::Vector3f& cameraPosition, sf::Vector3f& direction, float min_t, float max_t, Object* origin)
{
	auto closest = std::pair<Object*, float>(nullptr, Infinity);

	auto objects = scene->getSceneObjects();

	for (int i = 0; i < objects->size(); i++) {
		Object *object = objects->at(i);

		if (origin == object) continue;

		auto result = object->insertRay(cameraPosition, direction);

		if (result.x < closest.second && min_t < result.x && result.x < max_t) {
			closest.second = result.x;
			closest.first = objects->at(i);
		}
		if (result.y < closest.second && min_t < result.y && result.y < max_t) {
			closest.second = result.y;
			closest.first = objects->at(i);
		}
	}

	return closest;
}

sf::Vector3f Render::getReflectRay(const sf::Vector3f& const v1, const sf::Vector3f& const v2)
{
	return 2.f * v2 * Math::GetDotProduct(v2, v1) - v1;
}

void Render::calculate(Scene* scene)
{
	auto scale = this->viewport->getSize();
	auto pixels = this->viewport->getPixels();
	auto cameraPosition = scene->getCamera()->getPosition();

	for (int x = -scale.x / 2; x < scale.x / 2; x++) {
		for (int y = -scale.y / 2; y < scale.y / 2; y++) {
			auto direction = this->calculateDirection(x, y);

			sf::Color color = this->traceRay(scene, cameraPosition, direction, 1, Infinity, this->reflection_depth);
			this->viewport->updatePixel(scale.x / 2 + x, scale.y / 2 - y - 1, color);
		}
	}
}


void Render::update(Scene* scene, sf::RenderWindow& window, sf::Time time)
{
	this->calculate(scene);
	this->viewport->update(window);
}
