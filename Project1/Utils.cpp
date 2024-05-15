#include "./Utils.h"

TriangleMesh* Utils::LoadCustomFormatFile(const std::string const& filename)
{
    //std::ifstream ifs;

    //try {
    //    ifs.open(filename);

    //    if (ifs.fail()) throw;

    //    std::stringstream ss;

    //    ss << ifs.rdbuf();

    //    uint32_t numFaces;

    //    ss >> numFaces;

    //    std::unique_ptr<uint32_t[]> faceIndex(new uint32_t[numFaces]);

    //    uint32_t vertsIndexArraySize = 0;
    //    // reading face index array
    //    for (uint32_t i = 0; i < numFaces; ++i) {
    //        ss >> faceIndex[i];
    //        vertsIndexArraySize += faceIndex[i];
    //    }

    //    std::unique_ptr<uint32_t[]> vertsIndex(new uint32_t[vertsIndexArraySize]);
    //    uint32_t vertsArraySize = 0;

    //    // reading vertex index array
    //    for (uint32_t i = 0; i < vertsIndexArraySize; ++i) {
    //        ss >> vertsIndex[i];
    //        if (vertsIndex[i] > vertsArraySize) vertsArraySize = vertsIndex[i];
    //    }

    //    vertsArraySize += 1;

    //    // reading vertices
    //    std::unique_ptr<glm::vec3[]> verts(new glm::vec3[vertsArraySize]);

    //    for (uint32_t i = 0; i < vertsArraySize; ++i) {
    //        ss >> verts[i].x >> verts[i].y >> verts[i].z;
    //    }

    //    // reading normals
    //    std::unique_ptr<glm::vec3[]> normals(new glm::vec3[vertsIndexArraySize]);

    //    for (uint32_t i = 0; i < vertsIndexArraySize; ++i) {
    //        ss >> normals[i].x >> normals[i].y >> normals[i].z;
    //    }
    //    // reading st coordinates
    //    std::unique_ptr<glm::vec2[]> st(new glm::vec2[vertsIndexArraySize]);

    //    for (uint32_t i = 0; i < vertsIndexArraySize; ++i) {
    //        ss >> st[i].x >> st[i].y;
    //    }

    //    return new TriangleMesh(numFaces, faceIndex, vertsIndex, verts);
    //}
    //catch (...) {
    //    ifs.close();
    //}
    //ifs.close();

    return nullptr;
}

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
