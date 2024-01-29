#include "structs.h"

__kernel void GetPerPixel (
    //__global Render* render,
    //__global Scene* scene,
    //__global Matrix4d* rotation,
    //__global sf::Vector3f* cameraPosition,
    //float x,
    //float y,
    __global float* output
    )
{   
    unsigned int id = get_global_id(0);
    C99Color color;
    //sf::Color color = render->perPixel(x, y, scene, rotation, cameraPosition);
    //Color color(1,1,1,255);

    output[id] = sqrt(121.0f);
    //output[id] = 1;
    //output[id] = sf::Color(1,1,1, 255);
    //output[y][x]->setColor(color);
    //Get index into global data array
    //int iJob = get_global_id(0);

    ////Check boundary conditions
    //if (iJob >= elementsNumber) return; 

    ////Perform calculations
    //pOutputVectorHost[iJob] = MathCalculations(pInputVector1[iJob], pInputVector2[iJob]);
}