#pragma once
class AmbientLight
{
public:
	AmbientLight(float bright);

	float getBright();
private:
	float bright = 0;
};

