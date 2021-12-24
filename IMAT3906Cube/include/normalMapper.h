#pragma once


#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 norm;
	glm::vec2 uv;
	glm::vec3 tan;
	glm::vec3 binorm;
};

class normalMapper
{
private:
	std::vector<Vertex> vertices;
	std::vector<float> newVertData;
	void unpackData();
	void extractVertices(float* vertexData, int length);
	void getValues(unsigned int* indicesdata, int indicesLength);
	
public:
	normalMapper();
	void calctanandbinorm(float* vertexData, int length, unsigned int* indicesdata, int indicesLength);
	std::vector<float> getVertexData();


};