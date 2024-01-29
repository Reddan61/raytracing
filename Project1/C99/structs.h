#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

struct C99Color {
    int r,g,b;
};

typedef struct C99Color C99Color;

struct C99Vector {
    float x,y,z;
};

typedef struct C99Vector C99Vector;

struct C99Sphere {
    C99Vector position;
    float radius, specular, reflective;
    C99Color color;
};

typedef struct C99Sphere C99Sphere;

struct C99AmbientLight {
    float bright;
};

typedef struct C99AmbientLight C99AmbientLight;

struct C99DirectionalLight {
    float bright, maxT;
    C99Vector position;
};

typedef struct C99DirectionalLight C99DirectionalLight;

struct C99PointLight {
    float bright, maxT;
    C99Vector position;
};

typedef struct C99PointLight C99PointLight;

struct C99Scene {
    C99Sphere *spheres;
    int spheres_num;

    int numDirectionalLights, numPointLights;
    C99DirectionalLight *directionLights;
    C99AmbientLight *ambientLight;
    C99PointLight *pointLights;
};

typedef struct C99Scene C99Scene;

struct C99InsertRayValue {
    float t1, t2;
    C99Sphere *sphere;
};

typedef struct C99InsertRayValue C99InsertRayValue;

#endif // STRUCTS_H_INCLUDED
