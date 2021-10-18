#pragma once

class Entity
{
private:

public:
	virtual int Verticies() = 0;
	virtual void setData() = 0;
	virtual void getData() = 0;
	virtual void render() = 0;
	virtual void create() = 0;
};