#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <stb_image.h>

#include "Shader.h"
#include "Camera.h"
#include "SkyBox.h"

#include<string>
#include <iostream>
#include <numeric>
#include "normalMapper.h"




// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

unsigned int loadTexture(const char* path);
void loadTextureFiles();
void setFBOcolour();
void setFBODepth();
void setFBOcolourAndDepth();
void setFBOblur();
void setShadowMapFBO();
void createQuad();
void drawQuad(Shader& shader, unsigned int textureObj);
void drawQuad(Shader& shader, unsigned int textureObj, unsigned int Texobj);
void drawQuad(Shader& shader, unsigned int textureObj, unsigned int Texobj,unsigned int depth);
void setShader(Shader& shader);
void renderCubes(Shader& shader);
void renderPlane(Shader& shader);
void renderScene(Shader& shader, Shader& floorShader, Shader& skyBoxShader, Camera& cam);
void pointShadowCubeMap();
// camera
Camera camera(glm::vec3(0,0,9));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
SkyBox skybox;
//arrays
unsigned int floorVBO, cubeVBO, floorEBO, cubeEBO, cubeVAO, floorVAO,myFBO,FBO_depth,FBO_cAndD,FBO_blur,colourAttachment,cAttachment[2],depthAttachment,blurredTex,shadowMapFBO,shadowMap,quadVAO,quadVBO, crateTex,crateSpec,crateNorm,crateDisp, floorTex,floorSpec,floorNorm,floorDisp;
const unsigned int shadowWidth = 1024, shadowHeight = 1024;
unsigned int depthCubeMap;

normalMapper normalCubeMap;
normalMapper normalFloorMap;
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


float floorSize = 5.0f;
float floorLevel = -2.5f;

float floorVertices[] = {
		 -floorSize, floorLevel,  -floorSize, 0.0, 1.0, 0.0,   0.0f, 0.0f,
		 floorSize, floorLevel,   -floorSize, 0.0, 1.0, 0.0, 1.0f, 0.0f,
		 floorSize, floorLevel,  floorSize, 0.0, 1.0, 0.0, 1.0f,1.0f,
		-floorSize, floorLevel,  floorSize, 0.0, 1.0, 0.0, 0.0f,1.0f
};

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
	normalCubeMap.calctanandbinorm(cubeVertices, (192), cubeIndices, (36));
	std::vector<float>updatedCubeData = normalCubeMap.getVertexData();

	normalFloorMap.calctanandbinorm(floorVertices, (32), floorIndices, 6);
	std::vector<float>updatedFloorData = normalFloorMap.getVertexData();

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
	Shader floorShader("..\\shaders\\floorVert.vs", "..\\shaders\\floorFrag.fs");
	Shader postProcess("..\\shaders\\PP.vs","..\\shaders\\PP.fs");
	Shader depthPP("..\\shaders\\PP.vs", "..\\shaders\\dPP.fs");
	Shader blurShader("..\\shaders\\PP.vs", "..\\shaders\\blur.fs");
	Shader bloomShader("..\\shaders\\PP.vs", "..\\shaders\\bloom.fs");
	Shader DoFshader("..\\shaders\\PP.vs", "..\\shaders\\DoF.fs");
	Shader shadowMapshader("..\\shaders\\SM.vs", "..\\shaders\\SM.fs");
	Shader sbShader("..\\shaders\\skyBoxShader.vs", "..\\shaders\\skyBoxShader.fs");



	/*VAO stuff  - when you are comfortable what all of this is and what it is for - abstract to classes:
	  cube and floor class ( or plane class - can use for walls too!)*/
	  
	// Create VAO
	// Cube
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glGenBuffers(1, &cubeEBO);

	glBindVertexArray(cubeVAO);
	// fill VBO with vertex data
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, updatedCubeData.size()*sizeof(float), updatedCubeData.data(), GL_STATIC_DRAW);
	// fill EBO with index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//tangent attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 14* sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//binormal attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14* sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);
	//UV attribute
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 14* sizeof(float), (void*)(12 * sizeof(float)));
	glEnableVertexAttribArray(4);

	//Floor
	glGenVertexArrays(1, &floorVAO);
	glGenBuffers(1, &floorVBO);
	glGenBuffers(1, &floorEBO);

	glBindVertexArray(floorVAO);

	glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
	glBufferData(GL_ARRAY_BUFFER, updatedFloorData.size()*sizeof(float), updatedFloorData.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(floorIndices), floorIndices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// position attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);

	// position attribute
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14* sizeof(float), (void*)(9*sizeof(float)));
	glEnableVertexAttribArray(3);
	//UV attribute
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 14* sizeof(float), (void*)(12 * sizeof(float)));
	glEnableVertexAttribArray(4);

	skybox.createSkyBox();
	createQuad();
	glm::vec3 lightDir = glm::vec3(1.f,-0.7f,0.0f);
	//	glm::vec3 lightDir = glm::vec3(0.f,-0.7f,-1.0f);
	//glm::vec3 lightDir = glm::vec3(0.f, -1.0f, -0.01f);
	//glm::vec3 lightColour = glm::vec3(0.992f, 0.3687f, 0.3255f);
	//glm::vec3 lightColour = glm::vec3(1.f, 1.f, 1.f);
	glm::vec3 lightColour = glm::vec3(0.4f, 0.4f, 0.4f);
	loadTextureFiles();
	glm::vec3 cubeCol = glm::vec3(0.65f, 0.66f, 0.02f);
	glm::vec3 floorCol = glm::vec3(0.1f, 0.1f, 1.0f);
	shader.use();
	shader.setVec3("lightCol", lightColour);
	shader.setVec3("lightDirection", lightDir);
	floorShader.use();
	floorShader.setVec3("lightCol", lightColour);
	floorShader.setVec3("lightDirection", lightDir);
	depthPP.use();
	depthPP.setInt("image", 0);
	postProcess.use();
	postProcess.setInt("image", 0);
	blurShader.use();
	blurShader.setInt("image", 0);
	bloomShader.use();
	bloomShader.setInt("image", 0);
	bloomShader.setInt("bloomBlur", 1);
	DoFshader.use();
	DoFshader.setInt("image", 0);
	DoFshader.setInt("blur", 1);
	DoFshader.setInt("depth", 2);


	


	

	//setFBOcolour();
	setFBODepth();
	setFBOblur();
	setFBOcolourAndDepth();
	setShadowMapFBO();

	glm::vec3 lightPos = lightDir * glm::vec3(-1); //start +(end-start)*scalar
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.f, 20.f);
	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	shadowMapshader.use();
	shadowMapshader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
	shader.use();
	shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
	floorShader.use();
	floorShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
	



	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);
		//SCENE WITH BLURRING
		//renderScene(shader, floorShader, sbShader, camera);
		
		
		
		glBindFramebuffer(GL_FRAMEBUFFER, FBO_cAndD);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
		
		renderScene(shader, floorShader, sbShader, camera);
		
		glDisable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO_depth);
		drawQuad(depthPP, depthAttachment);
		
		renderScene(shader, floorShader, sbShader, camera);
		
		//blurring
		glBindFramebuffer(GL_FRAMEBUFFER, FBO_blur);
		
		blurShader.use();
		blurShader.setBool("horz", true);
		drawQuad(blurShader, cAttachment[1]);
		blurShader.setBool("horz", false);
		drawQuad(blurShader, blurredTex);
		
		
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//
		//drawQuad(bloomShader , cAttachment[1],blurredTex);
		////drawQuad(DoFshader, cAttachment[0], blurredTex, depthAttachment);
		//if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		//	drawQuad(postProcess, blurredTex);
		
			

		//SCENE WITH SHADOWS
		glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
		glViewport(0, 0, shadowWidth, shadowHeight);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		
		
		renderCubes(shadowMapshader);
		renderPlane(shadowMapshader);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();
		shader.setInt("shadowMap", 4);
		floorShader.use();
		floorShader.setInt("shadowMap", 4);
		
		
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, shadowMap);
		//drawQuad(bloomShader, cAttachment[0], blurredTex);
		renderScene(shader, floorShader, sbShader, camera);

		



		
		////SHADOW MAPS WORKING CORRECTLY
		//glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
		//glEnable(GL_DEPTH_TEST);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//renderCubes(shadowMapshader);
		//renderPlane(shadowMapshader);
		//
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glDisable(GL_DEPTH_TEST);
		//drawQuad(depthPP, shadowMap);

		//glViewport(0, 0, shadowWidth, shadowHeight);
		//glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
		//glClear(GL_DEPTH_BUFFER_BIT);
		//setShader(shader);
		//setShader(floorShader);
		//glBindTexture(GL_TEXTURE_CUBE_MAP,depthcu);
		//renderScene(shader,floorShader,sbShader,camera);
		
		
		
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

unsigned int loadTexture(const char* path)
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
	crateDisp= loadTexture("../Resources/Textures/Wood_Crate_001_SD/Wood_Crate_001_height.png");
	floorTex = loadTexture("../Resources/Textures/Wood_Planks_010_SD/Wood_Planks_010_COLOR.jpg");
	floorSpec = loadTexture("../Resources/Textures/Wood_Planks_010_SD/Wood_Planks_010_ROUGH.jpg");
	floorNorm = loadTexture("../Resources/Textures/Wood_Planks_010_SD/Wood_Planks_010_NORM.jpg");
	floorDisp = loadTexture("../Resources/Textures/Wood_Planks_010_SD/Wood_Planks_010_DISP.png"); 
}

void setFBOcolour()
{
	glGenFramebuffers(1, &myFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, myFBO);
	glGenTextures(1, &colourAttachment);

	
		glBindTexture(GL_TEXTURE_2D, colourAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA16F, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourAttachment, 0);
	



	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer not complete." << std::endl;
	}
}

void setFBODepth()
{
	glGenFramebuffers(1, &FBO_depth);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_depth);
	glGenTextures(1, &depthAttachment);
	glBindTexture(GL_TEXTURE_2D, depthAttachment);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void setFBOcolourAndDepth()
{
	glGenFramebuffers(1, &FBO_cAndD);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_cAndD);

	glGenTextures(2, cAttachment);
	for (int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, cAttachment[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, cAttachment[i], 0);
	}

	glGenTextures(1, &depthAttachment);
	glBindTexture(GL_TEXTURE_2D, depthAttachment);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0);
	
	unsigned int attachment[2] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachment);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}
void setFBOblur()
{
	glGenFramebuffers(1, &FBO_blur);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_blur);
	glGenTextures(1, &blurredTex);


	glBindTexture(GL_TEXTURE_2D, blurredTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurredTex, 0);
}
void setShadowMapFBO()
{
	glGenFramebuffers(1, &shadowMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);


	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void createQuad()
{
	float quadVertices[] =
	{
		-1.0f, 1.0f,0.0f,0.0f,1.0f,
		-1.0f,-1.0f,0.0f,0.0f,0.0f,
		1.0f,1.0f,0.0f,1.0f,1.0f,
		1.0f,-1.0f,0.0f,1.0f,0.0f
	};
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
}

void drawQuad(Shader& shader, unsigned int textureObj)
{
	shader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureObj);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
void drawQuad(Shader& shader, unsigned int textureObj, unsigned int Texobj)
{
	shader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureObj);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Texobj);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
void drawQuad(Shader& shader, unsigned int textureObj, unsigned int Texobj, unsigned int depth)
{
	shader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureObj);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Texobj);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, depth);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void setShader(Shader& shader)
{



	shader.setVec3("sLight.pos", glm::vec3(0.f, 10.f, -5.f));
	shader.setVec3("sLight.dir", glm::vec3(0.f, -1, 0.f));
	shader.setVec3("sLight.ambCol", glm::vec3(0.f, 0.f, 0.f));
	shader.setVec3("sLight.diffCol", glm::vec3(1.f, 1.f, 0.f));
	shader.setVec3("sLight.specCol", glm::vec3(1.f, 1.f, 0.f));
	shader.setFloat("sLight.kC", 1.f);
	shader.setFloat("sLight.lC", 0.05f);
	shader.setFloat("sLight.qC", 0.002f);
	shader.setFloat("sLight.inRad", glm::cos(glm::radians(10.f)));
	shader.setFloat("sLight.outRad", glm::cos(glm::radians(10.f)));
	shader.setFloat("sLight.ambFac", 0.f);
	shader.setFloat("sLight.specShine", 100.f);
	shader.setFloat("sLight.specStrength", 0.4);
	shader.setFloat("PXscale", 0.0175);

	shader.setVec3("pLight[0].position", glm::vec3(1, 0, 1));
	shader.setVec3("pLight[0].ambientCol", glm::vec3(0.2349520849, 0.8563239901, 0.2733646522));
	shader.setVec3("pLight[0].diffuseCol", glm::vec3(0.3123285777, 0.6348458802, 0.2858810024));
	shader.setVec3("pLight[0].specularCol", glm::vec3(0.4622860781, 0.2729412403, 0.0480364033));
	shader.setFloat("pLight[0].kC", 1.f);
	shader.setFloat("pLight[0].lC", 0.5116701455f);
	shader.setFloat("pLight[0].qC", 0.1815987919f);


	shader.setVec3( "pLight[1].position", glm::vec3(2, 0, -5));
	shader.setVec3( "pLight[1].ambientCol", glm::vec3(0.47477f, 0.62702f, 0.04221f));
	shader.setVec3( "pLight[1].diffuseCol", glm::vec3(0.79261f, 0.46497f, 0.71143f));
	shader.setVec3( "pLight[1].specularCol", glm::vec3(0.06035f, 0.87319f, 0.11919f));
	shader.setFloat("pLight[1].kC", 1.f);
	shader.setFloat("pLight[1].lC", 0.74308f);
	shader.setFloat("pLight[1].qC", 0.23647f);


	shader.setVec3( "pLight[2].position", glm::vec3(2, 1, -1.3));
	shader.setVec3( "pLight[2].ambientCol", glm::vec3(0.43886f, 0.15911f, 0.93682f));
	shader.setVec3( "pLight[2].diffuseCol", glm::vec3(0.97271f, 0.50532f, 0.28898f));
	shader.setVec3( "pLight[2].specularCol", glm::vec3(0.35509f, 0.74308f, 0.48367f));
	shader.setFloat("pLight[2].kC", 1.f);
	shader.setFloat("pLight[2].lC", 0.28180f);
	shader.setFloat("pLight[2].qC", 0.10973);


	shader.setVec3( "pLight[3].position", glm::vec3(-1, 2, 2));
	shader.setVec3( "pLight[3].ambientCol", glm::vec3(0.56755f, 0.30263f, 0.67051f));
	shader.setVec3( "pLight[3].diffuseCol", glm::vec3(0.51405f, 0.29218f, 0.26801f));
	shader.setVec3( "pLight[3].specularCol", glm::vec3(0.74798f, 0.41856f, 0.51405f));
	shader.setFloat("pLight[3].kC", 1.f);
	shader.setFloat("pLight[3].lC", 0.48788f);
	shader.setFloat("pLight[3].qC", 0.16726f);



}
void renderCubes(Shader& shader)
{
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);

	shader.use();
	shader.setInt("crateTex", 0);
	shader.setInt("crateSpec", 1);
	shader.setInt("crateNorm", 2);
	shader.setInt("crateDisp", 3);


	// MVP 

	// set uniforms - why do we set this each frame?

	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setMat4("model", model);
	shader.setVec3("viewPos", camera.Position);
	shader.setBool("map", map);



	setShader(shader);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, crateTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, crateSpec);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, crateNorm);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, crateDisp);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);   // what happens if we change to GL_LINE?
	glBindVertexArray(cubeVAO);  // bind and draw cube
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	model = glm::translate(model, glm::vec3(0.f, 0.f, 5.f));
	shader.setMat4("model", model);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.f, 0.f, -5.f));
	//model = glm::rotate(model, (float)glfwGetTime()*10000.f, glm::vec3(10.f, 10.f, 10.f));
	model = glm::rotate(model, (float)glfwGetTime() * .25f, glm::vec3(10.f, 10.f, 10.f));
	model = glm::scale(model, glm::vec3(5));

	shader.setMat4("model", model);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void renderPlane(Shader& shader)
{

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);

	shader.use();


	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setMat4("model", model);
	shader.setVec3("viewPos", camera.Position);
	shader.setInt("map", map);
	setShader(shader);


	shader.setInt("floorTex", 0);
	shader.setInt("floorSpec", 1);
	shader.setInt("floorNorm", 2);
	shader.setInt("floorDisp", 3);

	model = glm::mat4(1.0f);
	shader.setMat4("model", model);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, floorTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, floorSpec);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, floorNorm);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, floorDisp);

	glBindVertexArray(floorVAO);  // bind and draw floor
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.5f, 0.f, 0.f));
	model = glm::rotate(model,float(glm::radians(90.f)), glm::vec3(1, 0, 0));
	model = glm::rotate(model, float(glm::radians(90.f)), glm::vec3(0, 0, 1));
	shader.setMat4("model", model);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
void renderScene(Shader& shader, Shader& floorShader, Shader& skyBoxShader, Camera& cam)
{
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);

	shader.use();
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setMat4("model", model);
	shader.setVec3("viewPos", cam.Position);
	
	floorShader.use();
	floorShader.setMat4("projection", projection);
	floorShader.setMat4("view", view);
	floorShader.setMat4("model", model);
	floorShader.setVec3("viewPos", cam.Position);

	skyBoxShader.use();
	skyBoxShader.setMat4("projection", projection);
	skyBoxShader.setMat4("view", glm::mat4(glm::mat3(cam.GetViewMatrix())));

	skybox.renderSkyBox(skyBoxShader);
	renderCubes(shader);
	renderPlane(floorShader);
}

void pointShadowCubeMap()
{
	for (unsigned int i = 0; i < 6; i++)
	{
		GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, face, depthCubeMap,0);
		
	}

	
}


