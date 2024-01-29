#ifndef CONSTRUCTORS_H_INCLUDED
#define CONSTRUCTORS_H_INCLUDED

#include "./structs.h"

C99Color ConstructColor(float r, float g, float b);

C99Color ConstructColorFromVec(C99Vector vec);

C99Vector ConstructVector(float x, float y, float z);

C99Sphere ConstructSphere(
    C99Vector position,
    float radius,
    float specular,
    float reflective,
    C99Color* color
);

C99Scene ConstructScene(
    C99Sphere* spheres,
    int spheres_num,
    C99AmbientLight* ambientLight,
    C99PointLight* pointLights,
    C99DirectionalLight* directionalLights,
    int numDirectionalLights,
    int numPointLights
);

C99InsertRayValue ConstructInsertRayValue(float t1, float t2, C99Sphere* sphere);

C99AmbientLight ConstructAmbientLight(float bright);

C99PointLight ConstructPointLight(float bright, C99Vector* position);

C99DirectionalLight ConstructDirectionalLight(float bright, C99Vector* position);

#endif