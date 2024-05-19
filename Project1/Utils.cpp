#include "./Utils.h"

std::vector<Triangle*>* Utils::loadOBJ(const std::string& filename)
{
    float specular = -1.0f;
    float reflective = 0.0f;

    std::vector<Triangle*>* triangles = new std::vector<Triangle*>();

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << filename << std::endl;
        return triangles;
    }

    std::vector<glm::vec3> vertices;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
        else if (type == "f") {
            std::vector<int> indices;
            std::string vertexStr;
            while (iss >> vertexStr) {
                std::istringstream vertexIss(vertexStr);
                std::string indexStr;
                std::getline(vertexIss, indexStr, '/');
                int vertexIndex = std::stoi(indexStr);
                indices.push_back(vertexIndex - 1); // OBJ indices start from 1
            }
            if (indices.size() == 3) {
                Triangle* triangle = new Triangle(
                    vertices[indices[0]],
                    vertices[indices[1]],
                    vertices[indices[2]],
                    glm::vec3(0.0, 0.0, 1.0),
                    specular,
                    reflective
                );
                triangles->push_back(triangle);
            }
            else if(indices.size() == 4) {
                Triangle* triangle1 = new Triangle(
                    vertices[indices[0]],
                    vertices[indices[1]],
                    vertices[indices[2]],
                    glm::vec3(0.0, 0.0, 1.0),
                    specular,
                    reflective
                );
                Triangle* triangle2 = new Triangle(
                    vertices[indices[2]],
                    vertices[indices[3]],
                    vertices[indices[0]],
                    glm::vec3(0.0, 0.0, 1.0),
                    specular,
                    reflective
                );
                triangles->push_back(triangle1);
                triangles->push_back(triangle2);
            }
        }
    } 

    return triangles;
}
