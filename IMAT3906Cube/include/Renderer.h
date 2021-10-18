#pragma once
#include "Cube.h"
#include "Plane.h"

class Renderer
{
private:
	void renderCube();
	void renderPlanes();

	

	unsigned int cubeVAO, planeVAO, VBO, EBO;
public:
	Renderer();
	void renderScene();
};
