#include "./constructors.h"

C99Color ConstructColor(float r, float g, float b)
{
    C99Color color;

    color.r = r > 255 ? 255 : r;
    color.g = g > 255 ? 255 : g;
    color.b = b > 255 ? 255 : b;

    return color;
};

C99Color ConstructColorFromVec(C99Vector vec)
{
    C99Color color = ConstructColor(vec.x, vec.y, vec.z);

    return color;
};

C99Vector ConstructVector(float x, float y, float z)
{
    C99Vector vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;

    return vec;
};

C99Sphere ConstructSphere(
    C99Vector position,
    float radius,
    float specular,
    float reflective,
    C99Color* color
)
{
    C99Sphere result;

    result.position = ConstructVector(position.x, position.y, position.z);
    result.radius = radius;
    result.reflective = reflective;
    result.specular = specular;
    result.color = *color;

    return result;
}

C99Scene ConstructScene(
    C99Sphere* spheres,
    int spheres_num,
    C99AmbientLight* ambientLight,
    C99PointLight* pointLights,
    C99DirectionalLight* directionalLights,
    int numDirectionalLights,
    int numPointLights
)
{
    C99Scene result;

    result.spheres = spheres;
    result.spheres_num = spheres_num;
    result.ambientLight = ambientLight;
    result.directionLights = directionalLights;
    result.pointLights = pointLights;

    result.numDirectionalLights = numDirectionalLights;
    result.numPointLights = numPointLights;

    return result;
}

C99InsertRayValue ConstructInsertRayValue(float t1, float t2, C99Sphere* sphere) {
    C99InsertRayValue result;

    result.t1 = t1;
    result.t2 = t2;
    result.sphere = sphere;

    return result;
};

C99AmbientLight ConstructAmbientLight(float bright) {
    C99AmbientLight result;

    result.bright = bright;

    return result;
};

C99PointLight ConstructPointLight(float bright, C99Vector* position) {
    C99PointLight result;

    result.bright = bright;
    result.position = *position;
    result.maxT = 1.0f;

    return result;
};

C99DirectionalLight ConstructDirectionalLight(float bright, C99Vector* position) {
    C99DirectionalLight result;

    result.bright = bright;
    result.position = *position;
    result.maxT = INFINITY;

    return result;
};