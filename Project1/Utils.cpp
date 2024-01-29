#include "./Utils.h"

TriangleMesh* Utils::LoadCustomFormatFile(const std::string const& filename)
{
    std::ifstream ifs;

    try {
        ifs.open(filename);

        if (ifs.fail()) throw;

        std::stringstream ss;

        ss << ifs.rdbuf();

        uint32_t numFaces;

        ss >> numFaces;

        std::unique_ptr<uint32_t[]> faceIndex(new uint32_t[numFaces]);

        uint32_t vertsIndexArraySize = 0;
        // reading face index array
        for (uint32_t i = 0; i < numFaces; ++i) {
            ss >> faceIndex[i];
            vertsIndexArraySize += faceIndex[i];
        }

        std::unique_ptr<uint32_t[]> vertsIndex(new uint32_t[vertsIndexArraySize]);
        uint32_t vertsArraySize = 0;

        // reading vertex index array
        for (uint32_t i = 0; i < vertsIndexArraySize; ++i) {
            ss >> vertsIndex[i];
            if (vertsIndex[i] > vertsArraySize) vertsArraySize = vertsIndex[i];
        }

        vertsArraySize += 1;

        // reading vertices
        std::unique_ptr<Vector3d[]> verts(new Vector3d[vertsArraySize]);

        for (uint32_t i = 0; i < vertsArraySize; ++i) {
            ss >> verts[i].x >> verts[i].y >> verts[i].z;
        }

        // reading normals
        std::unique_ptr<Vector3d[]> normals(new Vector3d[vertsIndexArraySize]);

        for (uint32_t i = 0; i < vertsIndexArraySize; ++i) {
            ss >> normals[i].x >> normals[i].y >> normals[i].z;
        }
        // reading st coordinates
        std::unique_ptr<sf::Vector2f[]> st(new sf::Vector2f[vertsIndexArraySize]);

        for (uint32_t i = 0; i < vertsIndexArraySize; ++i) {
            ss >> st[i].x >> st[i].y;
        }

        return new TriangleMesh(numFaces, faceIndex, vertsIndex, verts, normals, st);
    }
    catch (...) {
        ifs.close();
    }
    ifs.close();

    return nullptr;
}
