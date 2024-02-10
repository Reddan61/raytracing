#include "./utils.cl"

#define NULLPTR ((void*)0)
#define EPSILON 0.00001f


C99Vector GetRayDirection(float x, float y)
{
    int width = 1200;
    int height = 800;

    return ConstructVector(x * 1.f / width, y * 1.f / height, 1.f);
};

C99InsertRayValue insertSphere(
    __global C99Vector* origin,
    C99Vector* dir,
    C99Sphere* sphere
) {
    C99Vector oc = VectorSubtractWithLeftGlobal(origin, &sphere->position);

    float a = DotProduct(dir, dir);
    float b = 2.f * DotProduct(&oc, dir);
    float c = DotProduct(&oc, &oc) - sphere->radius * sphere->radius;

    float discriminant = b * b - 4.f * a * c;

    if (discriminant < 0) {
        return ConstructInsertRayValue(INFINITY, INFINITY, NULLPTR);
    }

    float x1 = (-b + sqrt(discriminant)) / (2 * a);
    float x2 = (-b - sqrt(discriminant)) / (2 * a);

    return ConstructInsertRayValue(x1, x2, sphere);
};
C99InsertRayValue insertSpherePrivate(
    C99Vector* origin,
    C99Vector* dir,
    C99Sphere* sphere
) {
    C99Vector oc = VectorSubtract(origin, &sphere->position);

    float a = DotProduct(dir, dir);
    float b = 2.f * DotProduct(&oc, dir);
    float c = DotProduct(&oc, &oc) - sphere->radius * sphere->radius;

    float discriminant = b * b - 4.f * a * c;

    if (discriminant < 0) {
        return ConstructInsertRayValue(INFINITY, INFINITY, NULLPTR);
    }

    float x1 = (-b + sqrt(discriminant)) / (2 * a);
    float x2 = (-b - sqrt(discriminant)) / (2 * a);

    return ConstructInsertRayValue(x1, x2, sphere);
};

C99InsertRayValue getClosestObject(
    __global C99Scene* scene,
    __global C99Vector* origin,
    C99Vector* dir,
    float min_t,
    float max_t
) {

    C99InsertRayValue result = ConstructInsertRayValue(INFINITY, INFINITY, NULLPTR);

    if (scene->spheres_num > 0) {
        C99Sphere* spheres = scene->spheres;

        for (int i = 0; i < scene->spheres_num; i++) {
            C99Sphere sphere = spheres[i];

            C99InsertRayValue intersection = insertSphere(origin, dir, &sphere);

            if (intersection.t1 < result.t1 && min_t < intersection.t1 && intersection.t1 < max_t) {
                result = intersection;
                result.t2 = intersection.t1;
            }
            if (intersection.t2 < result.t2 && min_t < intersection.t2 && intersection.t2 < max_t) {
                result = intersection;
                result.t1 = intersection.t2;
            }
        }
    }

    return result;
};

C99InsertRayValue getClosestObjectPoint(
    __global C99Scene* scene,
    C99Vector* point,
    C99Vector* dir,
    float min_t,
    float max_t
) {

    C99InsertRayValue result = ConstructInsertRayValue(INFINITY, INFINITY, NULLPTR);
    
    if (scene->spheres_num > 0) {
        C99Sphere* spheres = scene->spheres;

        for (int i = 0; i < scene->spheres_num; i++) {
            C99Sphere sphere = spheres[i];

            C99InsertRayValue intersection = insertSpherePrivate(point, dir, &sphere);

            if (intersection.t1 < result.t1 && min_t < intersection.t1 && intersection.t1 < max_t) {
                result = intersection;
                result.t2 = intersection.t1;
            }
            if (intersection.t2 < result.t2 && min_t < intersection.t2 && intersection.t2 < max_t) {
                result = intersection;
                result.t1 = intersection.t2;
            }
        }
    }


    return result;
};

C99Vector GetReflectRay(C99Vector* v1, C99Vector* v2)
{
    C99Vector temp = VectorMultiplyByFloat(v2, 2.f);
    temp = VectorMultiplyByFloat(&temp, DotProduct(v2, v1));

    return VectorSubtract(&temp, v1);
}

float ComputeLight(
    __global C99Scene* scene,
    C99Vector* point,
    C99Vector* normal,
    C99Vector* view,
    float specular
) {
    float bright = 0;
 /*   float length_n = GetLengthVector(normal);
    float length_v = GetLengthVector(view);*/

    //if (scene->ambientLight != NULLPTR) {
    //    bright += scene->ambientLight->bright;
    //}

    //if (scene->numPointLights > 0) {
    //    for (int i = 0; i < scene->numPointLights; i++) {
    //        C99PointLight pointLight = scene->pointLights[i];

    //        C99Vector lightVector = GetPointLightVector(point, &pointLight);
    //        float maxT = pointLight.maxT;

    //        C99InsertRayValue shadow_blocker = getClosestObjectPoint(
    //            scene,
    //            point,
    //            &lightVector,
    //            EPSILON,
    //            maxT
    //        );
    //        if (shadow_blocker.sphere == NULLPTR) {
    //            continue;
    //        }

    //        float n_dot_l = DotProduct(normal, &lightVector);

    //        // Diffuse reflection
    //        if (n_dot_l > 0) {
    //            bright += pointLight.bright * n_dot_l / (length_n * GetLengthVector(&lightVector));
    //        }

    //        // Specular reflection.

    //        if (specular != -1) {
    //            C99Vector vec_r = GetReflectRay(&lightVector, normal);
    //            float r_dot_v = DotProduct(&vec_r, view);

    //            if (r_dot_v > 0) {
    //                bright += pointLight.bright * pow(r_dot_v / (GetLengthVector(&vec_r) * length_v), specular);
    //            }
    //        }
    //    }
    //}

    return bright;
};

C99Color traceRayWithPrivate(
    __global C99Scene* scene,
    C99Vector* origin,
    C99Vector* dir,
    float min_t,
    float max_t,
    int reflection_depth
) {
    C99InsertRayValue intersection = getClosestObjectPoint(scene, origin, dir, min_t, max_t);

    if (intersection.sphere == NULLPTR) return ConstructColor(0, 0, 0);

    C99Vector temp = VectorMultiplyByFloat(dir, intersection.t1);
    C99Vector point = VectorAdd(origin, &temp);

    C99Vector normal = getNormalSphere(intersection.sphere, &point);
    C99Vector view = VectorMultiplyByFloat(dir, -1.f);

    float lighting = ComputeLight(scene, &point, &normal, &view, intersection.sphere->specular);
    C99Color local_color = ColorMultiplyByFloat(&intersection.sphere->color, lighting);

    if (intersection.sphere->reflective <= 0 || reflection_depth <= 0) {
        return local_color;
    }

    C99Vector reflected_ray = GetReflectRay(&view, &normal);
    C99Color reflected_color = traceRayWithPrivate(
        scene,
        &point,
        &reflected_ray,
        EPSILON, INFINITY,
        reflection_depth - 1
    );

    float reflective = intersection.sphere->reflective;

    C99Color temp_color = ColorMultiplyByFloat(&local_color, (1.f - reflective));
    C99Color temp_color_1 = ColorMultiplyByFloat(&reflected_color, reflective);

    return ColorAdd(&temp_color, &temp_color_1);
};

C99Color traceRay(
    __global C99Scene* scene,
    __global C99Vector* origin,
    C99Vector* dir,
    float min_t,
    float max_t,
    int reflection_depth
) {
    C99InsertRayValue intersection = getClosestObject(scene, origin, dir, min_t, max_t);

    if (intersection.sphere == NULLPTR) {
        C99Color color = ConstructColor(255, 250, 255);

        return color;
    }

    C99Color color = ConstructColor(255, 249, 255);

    return color;

   /* C99Vector temp = VectorMultiplyByFloat(dir, intersection.t1);
    C99Vector point = VectorAddWithLeftGlobal(origin, &temp);*/

    //C99Vector normal = getNormalSphere(intersection.sphere, &point);
    //C99Vector view = VectorMultiplyByFloat(dir, -1.f);
//---
    //C99Color color = ConstructColor(255, 255, 255);

    //return color;

    //float lighting = ComputeLight(scene, &point, &normal, &view, intersection.sphere->specular);
    //C99Color local_color = ColorMultiplyByFloat(&intersection.sphere->color, lighting);

    //return local_color;

  /*  if (intersection.sphere->reflective <= 0 || reflection_depth <= 0) {
        return local_color;
    }

    C99Vector reflected_ray = GetReflectRay(&view, &normal);
    C99Color reflected_color = traceRayWithPrivate(
        scene,
        &point,
        &reflected_ray,
        EPSILON, INFINITY,
        reflection_depth - 1
    );

    float reflective = intersection.sphere->reflective;

    C99Color temp_color = ColorMultiplyByFloat(&local_color, (1.f - reflective));
    C99Color temp_color_1 = ColorMultiplyByFloat(&reflected_color, reflective);

    return ColorAdd(&temp_color, &temp_color_1);*/
};

C99Color getPixelColor(float x, float y, __global C99Scene* scene, __global C99Vector* origin)
{
    C99Vector dir = GetRayDirection(x, y);

    dir.x = dir.x * 2.0f - 1.0f;
    dir.y = 1.0f - 2.0f * dir.y;

    //dir = rotation * normalize(dir);
    dir = normalize(&dir);

    C99Color color = traceRay(scene, origin, &dir, 1, INFINITY, 3);

    return color;
};

C99Color perPixel(int x, int y, __global C99Vector* origin, __global C99Scene* scene)
{
    float aa = 4.f;
    float half_aa = aa / 2;
    float partial_aa = 1.f / half_aa;

    C99Vector color = ConstructVector(0, 0, 0);

    float pixel_start_x = x - 0.5f;
    float pixel_start_y = y - 0.5f;

    for (int i = 0; i < half_aa; i++) {
        float u = pixel_start_x + (partial_aa * i);
        float v = pixel_start_y + (partial_aa * i);

        color = VectorAddWithColor(color, getPixelColor(u, v, scene, origin));
    }

    pixel_start_x = x + 0.5f;

    for (int i = 0; i < half_aa; i++) {
        float u = pixel_start_x - (partial_aa * i);
        float v = pixel_start_y + (partial_aa * i);

        color = VectorAddWithColor(color, getPixelColor(u, v, scene, origin));
    }

    C99Vector result_ns = VectorDivision(color, aa);

    return ConstructColorFromVec(result_ns);
};