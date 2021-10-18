#pragma once

class Plane :public Entity
{
private:

public:
	Plane();
	void render() override;
	void create() override;
};
