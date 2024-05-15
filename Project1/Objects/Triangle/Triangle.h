#pragma once
#include "../Object.h"

class Triangle : public Object
{
public:
	// std140
	struct TriangleShader {
		int single_side;
		float specular, reflective;
		alignas(16) glm::vec4 A, B, C;
		glm::vec4 color;
	};

	struct AABB {
		glm::vec4 min, max;
		void grow(AABB box) {
			min = glm::min(min, box.min);
			max = glm::max(max, box.max);
		};
		float area() {
			glm::vec4 size = max - min;
			return size.x * size.y + size.y * size.z + size.z * size.x;
 		}
		glm::vec4 center() {
			return (min + max) * 0.5f;
		}
	};

	Triangle(
		const glm::vec3 const & A, 
		const glm::vec3 const & B, 
		const glm::vec3 const & C, 
		const glm::vec3 const &color, 
		float specular = -1.0f, 
		float reflective = 0
	);
	~Triangle();

	glm::vec3 getNormal(const glm::vec3 const& point, const glm::vec3 const& direction) override;
	TriangleShader getShader();
	static VkDeviceSize getBufferSize();

	void update(GLFWwindow* window, float delta) override;
	void changePosition(const glm::vec3 const& position) override;
	void rotate(float xAngle, float yAngle, glm::vec4 center);

	glm::vec4 getCentroid();
	Triangle::AABB getAABB();
private:
	glm::vec4 A, B, C;
	glm::vec4 centroid;
	bool isSingleSide = false;

	void calculateCentroid();
};

