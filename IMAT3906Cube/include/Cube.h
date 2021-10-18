#pragma once
#include "Entity.h"
class Cube :public Entity
{
private:

public:
	Cube();
	void render() override;
	void create() override;
};
