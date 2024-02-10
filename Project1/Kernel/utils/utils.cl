#include "constructors.cl"

C99Vector VectorDivision(C99Vector vec, float num)
{
    C99Vector result = ConstructVector(
        vec.x / num,
        vec.y / num,
        vec.z / num
    );

    return result;
};

C99Vector VectorAddWithColor(C99Vector vec, C99Color color)
{
    C99Vector result = ConstructVector(
        vec.x + color.r,
        vec.y + color.g,
        vec.z + color.b
    );

    return result;
}

C99Vector VectorAdd(C99Vector* v1, C99Vector* v2)
{
    C99Vector result = ConstructVector(
        v1->x + v2->x,
        v1->y + v2->y,
        v1->z + v2->z
    );

    return result;
}

C99Vector VectorAddWithLeftGlobal(__global C99Vector* v1, C99Vector* v2)
{
    C99Vector result = ConstructVector(
        v1->x + v2->x,
        v1->y + v2->y,
        v1->z + v2->z
    );

    return result;
}

C99Vector VectorSubtract(C99Vector* left, C99Vector* right)
{
    C99Vector result = ConstructVector(
        left->x - right->x,
        left->y - right->y,
        left->z - right->z
    );

    return result;
}

C99Vector VectorSubtractWithLeftGlobal(__global C99Vector* left, C99Vector* right)
{
    C99Vector result = ConstructVector(
        left->x - right->x,
        left->y - right->y,
        left->z - right->z
    );

    return result;
}

C99Vector VectorMultiplyByFloat(C99Vector* vec, float num)
{
    return ConstructVector(
        vec->x * num,
        vec->y * num,
        vec->z * num
    );
}

C99Color ColorMultiplyByFloat(C99Color* col, float num)
{
    return ConstructColor(
        col->r * num,
        col->g * num,
        col->b * num
    );
}

C99Color ColorAdd(C99Color* col1, C99Color* col2)
{
    return ConstructColor(
        col1->r * col2->r,
        col1->g * col2->g,
        col1->b * col2->b
    );
}

float length(C99Vector* vec)
{
    return sqrt(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
};

C99Vector normalize(C99Vector* vec)
{
    float inv_length = 1.0f / length(vec);

    return VectorMultiplyByFloat(vec, inv_length);
};

float DotProduct(C99Vector* v1, C99Vector* v2) {
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
};

C99Vector getNormalSphere(C99Sphere* sphere, C99Vector* point) {
    C99Vector normal = VectorSubtract(point, &sphere->position);
    normal = normalize(&normal);

    return normal;
};

float GetLengthVector(C99Vector* vec)
{
    return sqrt(DotProduct(vec, vec));
}

C99Vector GetPointLightVector(C99Vector* point, C99PointLight* light) {
    return VectorSubtract(&light->position, point);
};

C99Vector GetDirectionalLightVector(C99PointLight* light) {
    return light->position;
};