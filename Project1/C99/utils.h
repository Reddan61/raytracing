#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "./structs.h"
#include "./constructors.h"

C99Vector VectorDivision(C99Vector vec, float num);

C99Vector VectorAddWithColor(C99Vector vec, C99Color color);

C99Vector VectorAdd(C99Vector* v1, C99Vector* v2);

C99Vector VectorSubtract(C99Vector* left, C99Vector* right);

C99Vector VectorMultiplyByFloat(C99Vector* vec, float num);

C99Color ColorMultiplyByFloat(C99Color* col, float num);

C99Color ColorAdd(C99Color* col1, C99Color* col2);

float length(C99Vector* vec);

C99Vector normalize(C99Vector *vec);

float DotProduct(C99Vector *v1, C99Vector *v2);

C99Vector getNormalSphere(C99Sphere *sphere, C99Vector *point);

float GetLengthVector(C99Vector* vec);

C99Vector GetPointLightVector(C99Vector *point, C99PointLight *light);

C99Vector GetDirectionalLightVector(C99PointLight *light);

#endif // UTILS_H_INCLUDED
