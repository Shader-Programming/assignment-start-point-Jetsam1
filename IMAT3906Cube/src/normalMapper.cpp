

#include "normalMapper.h"

normalMapper::normalMapper()
{
}

void normalMapper::calctanandbinorm(float* vertexData,int length,unsigned int* indicesdata,int indicesLength)
{
	extractVertices(vertexData, length);
	getValues(indicesdata, indicesLength);
	unpackData();

}

std::vector<float> normalMapper::getVertexData()
{
	return newVertData;
}

void normalMapper::unpackData()
{
	int stride = 14;
	for (Vertex v : vertices)
	{
		float vertexData[] =
		{
			v.pos.x,v.pos.y,v.pos.z,
			v.norm.x,v.norm.y,v.norm.z,
			v.tan.x,v.tan.y,v.tan.z,
			v.binorm.x,v.binorm.y,v.binorm.z,
			v.uv.x,v.uv.y
		};
		for (int i = 0; i < stride; i++)
		{
			newVertData.push_back(vertexData[i]);
		}
	}
}

void normalMapper::extractVertices(float* vertexData, int length)
{
	Vertex point;
	for (int i = 0; i < length; i = i + 8)
	{
		point.pos = glm::vec3(vertexData[i], vertexData[i + 1], vertexData[i + 2]);
		point.norm = glm::vec3(vertexData[i + 3], vertexData[i + 4], vertexData[i + 5]);
		point.tan = glm::vec3(0.f, 0.f, 0.f);
		point.binorm = glm::vec3(0.f, 0.f, 0.f);
		point.uv = glm::vec2(vertexData[i + 6], vertexData[i + 7]);
		vertices.push_back(point);
	}
}

void normalMapper::getValues(unsigned int* indicesdata, int indicesLength)
{
	for (int i = 0; i < indicesLength; i = i + 3)
	{
		Vertex& v0 = vertices.at(indicesdata[i]);
		Vertex& v1 = vertices.at(indicesdata[i+1]);
		Vertex& v2 = vertices.at(indicesdata[i+2]);

		glm::vec3 d1 = v1.pos-v0.pos;
		glm::vec3 d2 = v2.pos-v0.pos;

		glm::vec2 duv1 = v1.uv - v0.uv;
		glm::vec2 duv2 = v2.uv - v0.uv;

		float determinant = 1.0f/(duv1.x * duv2.y - duv1.y * duv2.x); //find the determinant of the matrix cause 
		
		
			glm::vec3 calctan = (d1 * duv2.y - d2 * duv1.y) * (determinant);
			glm::vec3 calcbiNorm = (d2 * duv1.x - d1 * duv2.x)*(determinant);

			v0.tan = v0.tan + calctan;
			v0.binorm = v0.binorm + calcbiNorm;

			v1.tan = v1.tan + calctan;
			v1.binorm = v1.binorm + calcbiNorm;

			v2.tan = v2.tan + calctan;
			v2.binorm = v2.binorm + calcbiNorm;




		

	}

	for (Vertex& V : vertices)
	{
		V.tan = glm::normalize(V.tan);
		V.binorm = glm::normalize(V.binorm);
	}
}
