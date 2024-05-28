#include "AmbientLight.h"

AmbientLight::AmbientLight(float bright) : Light(bright, 0)
{
}

float AmbientLight::getMaxT()
{
	return 0;
}
