#pragma once
#include <stb_image.h>

#include <Shader.h>
#include <Camera.h>

class SkyBox
{
private:
	void loadCubeMapTextures();
	unsigned int loadCubeMap(std::vector<std::string> faces);
	void createVAO();
	std::vector<std::string> faces;
	unsigned int skyBoxTexObj;
	unsigned int skyBoxVAO;

public:
	SkyBox();
	void createSkyBox();
	void renderSkyBox(Shader& shader);
};