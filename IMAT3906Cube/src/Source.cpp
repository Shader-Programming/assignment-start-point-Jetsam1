#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <stb_image.h>

#include "Shader.h"
#include "Camera.h"
#include "normalMapper.h"

#include<string>
#include <iostream>
#include <numeric>
#include <vector>




// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

unsigned int loadTexture(char const* path);
void loadTextureFiles();
// camera
Camera camera(glm::vec3(0,0,9));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//arrays
unsigned int floorVBO, cubeVBO, floorEBO, cubeEBO, cubeVAO, floorVAO, crateTex,crateSpec,crateNorm, floorTex,floorSpec,floorNorm;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool map = false;;

// cube data
float cubeVertices[] = {
	//back
	
		   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f,// 0 
		    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f, //1
		    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

			//front
		   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,//4
	   	    0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
		    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		   -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

			//left
		   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,//8
		   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

			//right
		   0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 0.0f,//12
		   0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		   0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
 	 	   0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

		   //bottom
		  -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,//16
		   0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		   0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		  -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,

	//top	
		  -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, //20
		   0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		   0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		  -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
};

unsigned int cubeIndices[] = {
	1,2,3,
	1,3,0,

	5,6,7,
	5,7,4,

	11,8,9,
	11,9,10,

	15,12,13,
	15,13,14,

	23,22,21,
	23,21,20,

	19,18,17,
	19,17,16



};
normalMapper normalMap;
normalMapper nMap;

float floorSize = 5.0f;
float floorLevel = -2.0f;

float floorVertices[] = {
		 -floorSize, floorLevel,  -floorSize, 0.0, 1.0, 0.0,   0.0f, 0.0f,
		 floorSize, floorLevel,   -floorSize, 0.0, 1.0, 0.0, 1.0f, 0.0f,
		 floorSize, floorLevel,  floorSize, 0.0, 1.0, 0.0, 1.0f,1.0f,
		-floorSize, floorLevel,  floorSize, 0.0, 1.0, 0.0, 0.0f,1.0f
};
/*
float floorVertices[] = {
		 -5.f, -5.f,  -5.f, 0.0, 1.0, 0.0,   0.0f, 0.0f,
		 5.f, -5.f,   -5.f, 0.0, 1.0, 0.0, 1.0f, 0.0f,
		 5.f, -5.f,  5.f, 0.0, 1.0, 0.0, 1.0f,1.0f,
		-5.f, -5.f,  5.f, 0.0, 1.0, 0.0, 0.0f,1.0f
};*/
unsigned int floorIndices[] = {
	3,2,1,
	3,1,0
};




int main()
{


	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "IMAT3907", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}



	// simple vertex and fragment shader 
	Shader shader("..\\shaders\\plainVert.vs", "..\\shaders\\plainFrag.fs");
	Shader floorShader("..\\shaders\\plainvert.vs", "..\\shaders\\floorFrag.fs");
	shader.use();
	floorShader.use();


	/*VAO stuff  - when you are comfortable what all of this is and what it is for - abstract to classes:
	  cube and floor class ( or plane class - can use for walls too!)*/
	  
	// Create VAO
	// Cube

	normalMap.calctanandbinorm(cubeVertices, (sizeof(cubeVertices))/4, cubeIndices, sizeof(cubeIndices)/4);
	std::vector<float> updatedCubeVertices = normalMap.getVertexData();

	nMap.calctanandbinorm(floorVertices, sizeof(floorVertices)/4, floorIndices, sizeof(floorIndices)/4);
	std::vector<float> updatedFloorVertices = nMap.getVertexData();


	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glGenBuffers(1, &cubeEBO);

	glBindVertexArray(cubeVAO);
	// fill VBO with vertex data
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, updatedCubeVertices.size()*sizeof(GL_FLOAT), updatedCubeVertices.data() , GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	// fill EBO with index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14* sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14* sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//UV attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14* sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//tan attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14* sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);
	//binormal attribute
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
	glEnableVertexAttribArray(4);



	//Floor
	glGenVertexArrays(1, &floorVAO);
	glGenBuffers(1, &floorVBO);
	glGenBuffers(1, &floorEBO);

	glBindVertexArray(floorVAO);

	glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
	glBufferData(GL_ARRAY_BUFFER, updatedFloorVertices.size()*sizeof(GL_FLOAT), updatedFloorVertices.data(), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorIndices), floorIndices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//UV attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//tan attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);
	//binormal attribute
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
	glEnableVertexAttribArray(4);

	glm::vec3 lightDir = glm::vec3(1.f,-0.7f,0.0f);
	glm::vec3 lightColour = glm::vec3(0.992f, 0.3687f, 0.3255f);
	loadTextureFiles();
	glm::vec3 cubeCol = glm::vec3(0.65f, 0.66f, 0.02f);
	glm::vec3 floorCol = glm::vec3(0.1f, 0.1f, 1.0f);
	shader.use();
	shader.setVec3("lightCol", lightColour);
	shader.setVec3("lightDirection", lightDir);
	floorShader.use();
	floorShader.setVec3("lightCol", lightColour);
	floorShader.setVec3("lightDirection", lightDir);
	

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	


		
		shader.setInt("crateTex", 0);
		shader.setInt("crateSpec", 1);
		shader.setInt("crateNorm", 2);
	

		// MVP 
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		// set uniforms - why do we set this each frame?

	    shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setMat4("model", model);
		shader.setVec3("viewPos", camera.Position);
		shader.setBool("map", map);
	
		floorShader.setMat4("projection", projection);
		floorShader.setMat4("view", view);
		floorShader.setMat4("model", model);
		floorShader.setVec3("viewPos", camera.Position);
		floorShader.setInt("map", map);
		
		
		shader.setVec3("objectCol", cubeCol);


		//setting uniforms for pointlight
		shader.setVec3("pLight.position", glm::vec3(1, 0, 1));
		shader.setVec3("pLight.ambientCol", glm::vec3(0.2349520849, 0.8563239901, 0.2733646522));
		shader.setVec3("pLight.diffuseCol", glm::vec3(0.3123285777, 0.6348458802, 0.2858810024));
		shader.setVec3("pLight.specularCol", glm::vec3(0.4622860781, 0.2729412403, 0.0480364033));
		shader.setFloat("pLight.kC", 1.f);
		shader.setFloat("pLight.lC", 0.5116701455f);
		shader.setFloat("pLight.qC", 0.1815987919f);  
		shader.setFloat("pLight.ambFac", 0.05f);
		shader.setFloat("pLight.specShine", 50.f);
		shader.setFloat("pLight.specStrength", 0.7);
		
		floorShader.setVec3("pLight.position", glm::vec3(1, 0, 1));
		floorShader.setVec3("pLight.ambientCol", glm::vec3(0.2349520849, 0.8563239901, 0.2733646522));
		floorShader.setVec3("pLight.diffuseCol", glm::vec3(0.3123285777, 0.6348458802, 0.2858810024));
		floorShader.setVec3("pLight.specularCol", glm::vec3(0.4622860781, 0.2729412403, 0.0480364033));
		floorShader.setFloat("pLight.kC", 1.f);
		floorShader.setFloat("pLight.lC", 0.5116701455f);
		floorShader.setFloat("pLight.qC", 0.1815987919f);
		floorShader.setFloat("pLight.ambFac", 0.05f);
		floorShader.setFloat("pLight.specShine", 50.f);
		floorShader.setFloat("pLight.specStrength", 0.7);
		
		
		//setting uniforms for spotLight
		shader.setVec3("sLight.pos", glm::vec3(0.f,10.f,-5.f));
		shader.setVec3("sLight.dir", glm::vec3(0.f,-1,0.f));
		shader.setVec3("sLight.ambCol", glm::vec3(0.f,0.f,0.f));
		shader.setVec3("sLight.diffCol", glm::vec3(1.f,1.f,0.f));
		shader.setVec3("sLight.specCol", glm::vec3(1.f,1.f,0.f));
		shader.setFloat("sLight.kC", 1.f);
		shader.setFloat("sLight.lC", 0.05f);
		shader.setFloat("sLight.qC", 0.002f);
		shader.setFloat("sLight.inRad", glm::cos(glm::radians(10.f)));
		shader.setFloat("sLight.outRad", glm::cos(glm::radians(10.f)));
		shader.setFloat("sLight.ambFac", 0.f);
		shader.setFloat("sLight.specShine", 100.f);
		shader.setFloat("sLight.specStrength", 0.4);

		floorShader.setVec3("sLight.pos", glm::vec3(0.f, 10.f, -5.f));
		floorShader.setVec3("sLight.dir", glm::vec3(0.f, -1, 0.f));
		floorShader.setVec3("sLight.ambCol", glm::vec3(0.f, 0.f, 0.f));
		floorShader.setVec3("sLight.diffCol", glm::vec3(1.f, 1.f, 0.f));
		floorShader.setVec3("sLight.specCol", glm::vec3(1.f, 1.f, 0.f));
		floorShader.setFloat("sLight.kC", 1.f);
		floorShader.setFloat("sLight.lC", 0.05f);
		floorShader.setFloat("sLight.qC", 0.002f);
		floorShader.setFloat("sLight.inRad", glm::cos(glm::radians(10.f)));
		floorShader.setFloat("sLight.outRad", glm::cos(glm::radians(10.f)));
		floorShader.setFloat("sLight.ambFac", 0.f);
		floorShader.setFloat("sLight.specShine", 100.f);
		floorShader.setFloat("sLight.specStrength", 0.4);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, crateTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, crateSpec);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, crateNorm);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);   // what happens if we change to GL_LINE?
		glBindVertexArray(cubeVAO);  // bind and draw cube
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		model = glm::translate(model, glm::vec3(0.f, 0.f, 5.f));
		shader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.f, 0.f, -5.f));
		//model = glm::rotate(model, (float)glfwGetTime()*10000.f, glm::vec3(10.f, 10.f, 10.f));
		model = glm::rotate(model, (float)glfwGetTime()*.25f, glm::vec3(10.f, 10.f, 10.f));
		model = glm::scale(model, glm::vec3(5));

		shader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		floorShader.setInt("floorTex", 3);
		floorShader.setInt("floorSpec", 4);
		floorShader.setInt("floorNorm", 5);
	
		model = glm::mat4(1.0f);
		floorShader.setMat4("model", model);
		floorShader.setVec3("objectCol", floorCol);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, floorTex);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, floorSpec);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, floorNorm);
		glBindVertexArray(floorVAO);  // bind and draw floor
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(MOVEUP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera.ProcessKeyboard(MOVEDOWN, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		if (map == true) map = 0;
		else map = 1;
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, comp;
	unsigned char* data = stbi_load(path, &width, &height, &comp, 0);

	if (data)
	{
		GLenum format;
		if (comp == 1)
			format = GL_RED;
		else if (comp == 3)
			format = GL_RGB;
		else if (comp == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //x axis
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //y axis
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
		std::cout << "Loaded texture from: " << path << " width: " << width << " height: " << height << " id " << textureID << std::endl;
	}
	else
	{
		std::cout << "Couldnt load texture from: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}


void loadTextureFiles()
{
	crateTex = loadTexture("../Resources/Textures/Wood_Crate_001_SD/Wood_Crate_001_basecolor.jpg");
	crateSpec = loadTexture("../Resources/Textures/Wood_Crate_001_SD/Wood_Crate_001_roughness.jpg");
	crateNorm = loadTexture("../Resources/Textures/Wood_Crate_001_SD/Wood_Crate_001_normal.jpg");
	floorTex = loadTexture("../Resources/Textures/Wood_Planks_010_SD/Wood_Planks_010_COLOR.jpg");
	floorSpec = loadTexture("../Resources/Textures/Wood_Planks_010_SD/Wood_Planks_010_DISP.png");
	floorNorm = loadTexture("../Resources/Textures/Wood_Planks_010_SD/Wood_Planks_010_NORM.jpg");
}




