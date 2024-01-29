#ifndef RAYTRACING_H_INCLUDED

#define RAYTRACING_H_INCLUDED

#include "./utils.h"

#define nullptr ((void*)0)
#define EPSILON 0.00001f


C99Vector GetRayDirection(float x, float y);

C99InsertRayValue insertSphere(
	C99Vector* origin,
	C99Vector* dir,
	C99Sphere* sphere
);

C99InsertRayValue getClosestObject(
	C99Scene* scene,
	C99Vector* origin,
	C99Vector* dir,
	float min_t,
	float max_t
);

C99Vector GetReflectRay(C99Vector* v1, C99Vector* v2);

float ComputeLight(C99Scene* scene,
	C99Vector* point,
	C99Vector* normal,
	C99Vector* view,
	float specular
);

C99Color traceRay(
	C99Scene* scene,
	C99Vector* origin,
	C99Vector* dir,
	float min_t,
	float max_t,
	int reflection_depth
);

C99Color getPixelColor(float x, float y, C99Scene* scene, C99Vector* origin);

C99Color perPixel(int x, int y, C99Vector* origin, C99Scene* scene);

#endif