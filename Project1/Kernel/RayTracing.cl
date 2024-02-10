#include "raytracing.cl"

__kernel void RayTracing (
    __global C99Vector* origin,
    __global C99Scene* scene,
    int WIDTH,
    int HEIGHT,
    __global C99Color* output
    )
{   
    int gidX = get_global_id(0);
    int gidY = get_global_id(1);
    int colCount = get_global_size(0);

    int index = gidY * colCount + gidX;

    C99Color color = perPixel(gidX, gidY, origin, scene);
    //C99Color color = ConstructColor(255, 255, 0);
    
    output[index] = color;

    ////Check boundary conditions
    //if (iJob >= elementsNumber) return; 

    ////Perform calculations
    //pOutputVectorHost[iJob] = MathCalculations(pInputVector1[iJob], pInputVector2[iJob]);
}