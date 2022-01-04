#pragma once
#include <vector>

class Entity
{
private:

public:
	virtual std::vector<float> Vertices() = 0;
	virtual void setData() = 0;
	virtual void getData() = 0;
	virtual void render() = 0;
	virtual void create() = 0;
	unsigned int diffuse, specular, normal, displacement;
};