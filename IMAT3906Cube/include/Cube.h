#pragma once
#include "Entity.h"
#include "normalMapper.h"
#include "Shader.h"
#include "Textures.h"
class Cube :public Entity
{
private:
	float edgeSize;
	glm::vec2 centrePos;
	std::vector<float> vertices;
	std::array<int,36> indices();
public:
	std::vector<float> Vertices() override;
	Cube();
	void render() override;
	void create() override;
	void setData() override;
	void getData() override;
	unsigned int diffuse, specular, normal, displacement;
};
