//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "table_chair.h"
//#include "stool.h"
#include "fan.h"
#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;



// modelling transform
float rotateAngle_X = 0;
float rotateAngle_Y = 0;
float rotateAngle_Z = 0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;
bool fan_turn = false;
bool rotate_around = false;
// camera
Camera camera(glm::vec3(0.0f, 2.5f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

glm::mat4 transformation(float tx, float ty, float tz, float rx, float ry, float rz, float sx, float sy, float sz) {
	glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
	translateMatrix = glm::translate(identityMatrix, glm::vec3(tx, ty, tz));
	rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
	rotateYMatrix = glm::rotate(identityMatrix, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
	rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));
	scaleMatrix = glm::scale(identityMatrix, glm::vec3(sx, sy, sz));
	model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
	return model;
}




void cylinder(Shader& ourShader, unsigned int& VAOdec, float tx, float ty, float tz, float sx, float sy, float sz)
{
	glm::mat4 model;
	model = transformation(tx, ty, tz, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, sx, sy, sz);
	ourShader.setMat4("model", model);
	glBindVertexArray(VAOdec);
	glDrawElements(GL_TRIANGLES, 120, GL_UNSIGNED_INT, 0);
}

void glass(Shader& ourShader, unsigned int& VAO, float tx, float ty, float tz, float sx, float sy, float sz)
{
	glm::mat4 model;
	model = transformation(tx, ty, tz, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, sx, sy, sz);
	ourShader.setMat4("model", model);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 96, GL_UNSIGNED_INT, 0);
}

void bottle(Shader& ourShader, unsigned int& VAO, float tx, float ty, float tz, float sx, float sy, float sz)
{
	glm::mat4 model;
	model = transformation(tx, ty, tz, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, sx, sy, sz);
	ourShader.setMat4("model", model);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 138, GL_UNSIGNED_INT, 0);
}

void bulb(Shader& ourShader, unsigned int& VAO1, unsigned int& VAO2, unsigned int& VAO3, float tx, float ty, float tz)
{
	glm::mat4 model;
	model = transformation(tx+2, ty+1, tz+0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, .08, .2, .08);
	ourShader.setMat4("model", model);//bulb
	glBindVertexArray(VAO1);
	glDrawElements(GL_TRIANGLES, 138, GL_UNSIGNED_INT, 0);
	model = transformation(tx + 2, ty + 1.2, tz + 0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, .01, .7, .01);
	ourShader.setMat4("model", model);//rod
	glBindVertexArray(VAO2);
	glDrawElements(GL_TRIANGLES, 120, GL_UNSIGNED_INT, 0);
	model = transformation(tx + 1.925, ty + 1.9, tz -0.07, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, .3, .1, .3);
	ourShader.setMat4("model", model);//top_holder
	glBindVertexArray(VAO3);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
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

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	Shader ourShader("vertexShader.vs", "fragmentShader.fs");
	//0.59f, 0.19f, 0.0f,
	float table_top[] = {
		0.0f, 0.0f, 0.0f, 0.78f, 0.66f, 0.44f,
		0.5f, 0.0f, 0.0f, 0.78f, 0.66f, 0.44f,
		0.5f, 0.5f, 0.0f, 0.78f, 0.66f, 0.44f,
		0.0f, 0.5f, 0.0f, 0.78f, 0.66f, 0.44f,

		0.5f, 0.0f, 0.0f, 0.78f, 0.66f, 0.44f,
		0.5f, 0.5f, 0.0f, 0.78f, 0.66f, 0.44f,
		0.5f, 0.0f, 0.5f, 0.78f, 0.66f, 0.44f,
		0.5f, 0.5f, 0.5f, 0.78f, 0.66f, 0.44f,

		0.0f, 0.0f, 0.5f, 0.78f, 0.66f, 0.44f,
		0.5f, 0.0f, 0.5f, 0.78f, 0.66f, 0.44f,
		0.5f, 0.5f, 0.5f, 0.78f, 0.66f, 0.44f,
		0.0f, 0.5f, 0.5f, 0.78f, 0.66f, 0.44f,

		0.0f, 0.0f, 0.5f, 0.78f, 0.66f, 0.44f,
		0.0f, 0.5f, 0.5f, 0.78f, 0.66f, 0.44f,
		0.0f, 0.5f, 0.0f, 0.78f, 0.66f, 0.44f,
		0.0f, 0.0f, 0.0f, 0.78f, 0.66f, 0.44f,

		0.5f, 0.5f, 0.5f, 0.78f, 0.66f, 0.44f,
		0.5f, 0.5f, 0.0f, 0.78f, 0.66f, 0.44f,
		0.0f, 0.5f, 0.0f, 0.78f, 0.66f, 0.44f,
		0.0f, 0.5f, 0.5f, 0.78f, 0.66f, 0.44f,

		0.0f, 0.0f, 0.0f, 0.78f, 0.66f, 0.44f,
		0.5f, 0.0f, 0.0f, 0.78f, 0.66f, 0.44f,
		0.5f, 0.0f, 0.5f, 0.78f, 0.66f, 0.44f,
		0.0f, 0.0f, 0.5f, 0.78f, 0.66f, 0.44f,
	};
	//0.80f, 0.59f, 0.0f,

	float table_leg[] =   {
		0.0f, 0.0f, 0.0f,    0.984f, 0.926f, 0.852f,
		0.5f, 0.0f, 0.0f,     0.984f, 0.926f, 0.852f,
		0.5f, 0.5f, 0.0f,     0.984f, 0.926f, 0.852f,
		0.0f, 0.5f, 0.0f,    0.984f, 0.926f, 0.852f,

		0.5f, 0.0f, 0.0f,     0.984f, 0.926f, 0.852f,
		0.5f, 0.5f, 0.0f,     0.984f, 0.926f, 0.852f,
		0.5f, 0.0f, 0.5f,     0.984f, 0.926f, 0.852f,
		0.5f, 0.5f, 0.5f,     0.984f, 0.926f, 0.852f,

		0.0f, 0.0f, 0.5f,     0.984f, 0.926f, 0.852f,
		0.5f, 0.0f, 0.5f,    0.984f, 0.926f, 0.852f,
		0.5f, 0.5f, 0.5f,     0.984f, 0.926f, 0.852f,
		0.0f, 0.5f, 0.5f,     0.984f, 0.926f, 0.852f,

		0.0f, 0.0f, 0.5f,     0.984f, 0.926f, 0.852f,
		0.0f, 0.5f, 0.5f,     0.984f, 0.926f, 0.852f,
		0.0f, 0.5f, 0.0f,     0.984f, 0.926f, 0.852f,
		0.0f, 0.0f, 0.0f,     0.984f, 0.926f, 0.852f,

		0.5f, 0.5f, 0.5f,     0.984f, 0.926f, 0.852f,
		0.5f, 0.5f, 0.0f,    0.984f, 0.926f, 0.852f,
		0.0f, 0.5f, 0.0f,     0.984f, 0.926f, 0.852f,
		0.0f, 0.5f, 0.5f,     0.984f, 0.926f, 0.852f,

		0.0f, 0.0f, 0.0f,    0.984f, 0.926f, 0.852f,
		0.5f, 0.0f, 0.0f,    0.984f, 0.926f, 0.852f,
		0.5f, 0.0f, 0.5f,     0.984f, 0.926f, 0.852f,
		0.0f, 0.0f, 0.5f,     0.984f, 0.926f, 0.852f,
	};


	//chair seat
	float chair_leg[] = {
		0.0f, 0.0f, 0.0f, .543f, .19f, .215f,
		0.5f, 0.0f, 0.0f, .543f, .19f, .215f,
		0.5f, 0.5f, 0.0f, .543f, .19f, .215f,
		0.0f, 0.5f, 0.0f, .543f, .19f, .215f,

		0.5f, 0.0f, 0.0f, .543f, .19f, .215f,
		0.5f, 0.5f, 0.0f, .543f, .19f, .215f,
		0.5f, 0.0f, 0.5f, .543f, .19f, .215f,
		0.5f, 0.5f, 0.5f, .543f, .19f, .215f,

		0.0f, 0.0f, 0.5f, .543f, .19f, .215f,
		0.5f, 0.0f, 0.5f, .543f, .19f, .215f,
		0.5f, 0.5f, 0.5f, .543f, .19f, .215f,
		0.0f, 0.5f, 0.5f, .543f, .19f, .215f,

		0.0f, 0.0f, 0.5f, .543f, .19f, .215f,
		0.0f, 0.5f, 0.5f, .543f, .19f, .215f,
		0.0f, 0.5f, 0.0f, .543f, .19f, .215f,
		0.0f, 0.0f, 0.0f, .543f, .19f, .215f,

		0.5f, 0.5f, 0.5f, .543f, .19f, .215f,
		0.5f, 0.5f, 0.0f, .543f, .19f, .215f,
		0.0f, 0.5f, 0.0f, .543f, .19f, .215f,
		0.0f, 0.5f, 0.5f, .543f, .19f, .215f,

		0.0f, 0.0f, 0.0f, .543f, .19f, .215f,
		0.5f, 0.0f, 0.0f, .543f, .19f, .215f,
		0.5f, 0.0f, 0.5f, .543f, .19f, .215f,
		0.0f, 0.0f, 0.5f, .543f, .19f, .215f,
	};

	//background of board
	float chair_pillar[] = {
		0.0f, 0.0f, 0.0f, .0f, .0f, .0f,
		0.5f, 0.0f, 0.0f, .0f, .0f, .0f,
		0.5f, 0.5f, 0.0f, .0f, .0f, .0f,
		0.0f, 0.5f, 0.0f, .0f, .0f, .0f,

		0.5f, 0.0f, 0.0f, .0f, .0f, .0f,
		0.5f, 0.5f, 0.0f, .0f, .0f, .0f,
		0.5f, 0.0f, 0.5f, .0f, .0f, .0f,
		0.5f, 0.5f, 0.5f, .0f, .0f, .0f,

		0.0f, 0.0f, 0.5f, .0f, .0f, .0f,
		0.5f, 0.0f, 0.5f, .0f, .0f, .0f,
		0.5f, 0.5f, 0.5f, .0f, .0f, .0f,
		0.0f, 0.5f, 0.5f, .0f, .0f, .0f,

		0.0f, 0.0f, 0.5f, .0f, .0f, .0f,
		0.0f, 0.5f, 0.5f, .0f, .0f, .0f,
		0.0f, 0.5f, 0.0f, .0f, .0f, .0f,
		0.0f, 0.0f, 0.0f, .0f, .0f, .0f,

		0.5f, 0.5f, 0.5f, .0f, .0f, .0f,
		0.5f, 0.5f, 0.0f, .0f, .0f, .0f,
		0.0f, 0.5f, 0.0f, .0f, .0f, .0f,
		0.0f, 0.5f, 0.5f, .0f, .0f, .0f,

		0.0f, 0.0f, 0.0f, .0f, .0f, .0f,
		0.5f, 0.0f, 0.0f, .0f, .0f, .0f,
		0.5f, 0.0f, 0.5f, .0f, .0f, .0f,
		0.0f, 0.0f, 0.5f, .0f, .0f, .0f,
	};

	//0.9f, 0.9f, 0.0f,
	float chair_back[] = {
		0.0f, 0.0f, 0.0f, 0.684f, 0.238f, 0.27f,
		0.5f, 0.0f, 0.0f, 0.684f, 0.238f, 0.27f,
		0.5f, 0.5f, 0.0f, 0.684f, 0.238f, 0.27f,
		0.0f, 0.5f, 0.0f, 0.684f, 0.238f, 0.27f,

		0.5f, 0.0f, 0.0f, 0.684f, 0.238f, 0.27f,
		0.5f, 0.5f, 0.0f, 0.684f, 0.238f, 0.27f,
		0.5f, 0.0f, 0.5f, 0.684f, 0.238f, 0.27f,
		0.5f, 0.5f, 0.5f, 0.684f, 0.238f, 0.27f,

		0.0f, 0.0f, 0.5f, 0.684f, 0.238f, 0.27f,
		0.5f, 0.0f, 0.5f, 0.684f, 0.238f, 0.27f,
		0.5f, 0.5f, 0.5f, 0.684f, 0.238f, 0.27f,
		0.0f, 0.5f, 0.5f, 0.684f, 0.238f, 0.27f,

		0.0f, 0.0f, 0.5f, 0.684f, 0.238f, 0.27f,
		0.0f, 0.5f, 0.5f, 0.684f, 0.238f, 0.27f,
		0.0f, 0.5f, 0.0f, 0.684f, 0.238f, 0.27f,
		0.0f, 0.0f, 0.0f, 0.684f, 0.238f, 0.27f,

		0.5f, 0.5f, 0.5f, 0.684f, 0.238f, 0.27f,
		0.5f, 0.5f, 0.0f, 0.684f, 0.238f, 0.27f,
		0.0f, 0.5f, 0.0f, 0.684f, 0.238f, 0.27f,
		0.0f, 0.5f, 0.5f, 0.684f, 0.238f, 0.27f,

		0.0f, 0.0f, 0.0f, 0.684f, 0.238f, 0.27f,
		0.5f, 0.0f, 0.0f, 0.684f, 0.238f, 0.27f,
		0.5f, 0.0f, 0.5f, 0.684f, 0.238f, 0.27f,
		0.0f, 0.0f, 0.5f, 0.684f, 0.238f, 0.27f
	};
	//0.69f, 0.69f, 0.69f,
	float floor[] = {
		0.0f, 0.0f, 0.0f, 0.69f, 0.69f, 0.69f,
		0.5f, 0.0f, 0.0f, 0.69f, 0.69f, 0.69f,
		0.5f, 0.5f, 0.0f, 0.69f, 0.69f, 0.69f,
		0.0f, 0.5f, 0.0f, 0.69f, 0.69f, 0.69f,

		0.5f, 0.0f, 0.0f, 0.69f, 0.69f, 0.69f,
		0.5f, 0.5f, 0.0f, 0.69f, 0.69f, 0.69f,
		0.5f, 0.0f, 0.5f, 0.69f, 0.69f, 0.69f,
		0.5f, 0.5f, 0.5f, 0.69f, 0.69f, 0.69f,

		0.0f, 0.0f, 0.5f, 0.69f, 0.69f, 0.69f,
		0.5f, 0.0f, 0.5f, 0.69f, 0.69f, 0.69f,
		0.5f, 0.5f, 0.5f, 0.69f, 0.69f, 0.69f,
		0.0f, 0.5f, 0.5f, 0.69f, 0.69f, 0.69f,

		0.0f, 0.0f, 0.5f, 0.69f, 0.69f, 0.69f,
		0.0f, 0.5f, 0.5f, 0.69f, 0.69f, 0.69f,
		0.0f, 0.5f, 0.0f, 0.69f, 0.69f, 0.69f,
		0.0f, 0.0f, 0.0f, 0.69f, 0.69f, 0.69f,

		0.5f, 0.5f, 0.5f, 0.69f, 0.69f, 0.69f,
		0.5f, 0.5f, 0.0f, 0.69f, 0.69f, 0.69f,
		0.0f, 0.5f, 0.0f, 0.69f, 0.69f, 0.69f,
		0.0f, 0.5f, 0.5f, 0.69f, 0.69f, 0.69f,

		0.0f, 0.0f, 0.0f, 0.69f, 0.69f, 0.69f,
		0.5f, 0.0f, 0.0f, 0.69f, 0.69f, 0.69f,
		0.5f, 0.0f, 0.5f, 0.69f, 0.69f, 0.69f,
		0.0f, 0.0f, 0.5f, 0.69f, 0.69f, 0.69f
	};

	//front_back wall
	float wall1[] = {
		0.0f, 0.0f, 0.0f, 0.621f, 0.559f, 0.699f,
		0.5f, 0.0f, 0.0f, 0.621f, 0.559f, 0.699f,
		0.5f, 0.5f, 0.0f, 0.621f, 0.559f, 0.699f,
		0.0f, 0.5f, 0.0f, 0.621f, 0.559f, 0.699f,

		0.5f, 0.0f, 0.0f, 0.621f, 0.559f, 0.699f,
		0.5f, 0.5f, 0.0f, 0.621f, 0.559f, 0.699f,
		0.5f, 0.0f, 0.5f, 0.621f, 0.559f, 0.699f,
		0.5f, 0.5f, 0.5f, 0.621f, 0.559f, 0.699f,

		0.0f, 0.0f, 0.5f, 0.621f, 0.559f, 0.699f,
		0.5f, 0.0f, 0.5f, 0.621f, 0.559f, 0.699f,
		0.5f, 0.5f, 0.5f, 0.621f, 0.559f, 0.699f,
		0.0f, 0.5f, 0.5f, 0.621f, 0.559f, 0.699f,

		0.0f, 0.0f, 0.5f, 0.621f, 0.559f, 0.699f,
		0.0f, 0.5f, 0.5f, 0.621f, 0.559f, 0.699f,
		0.0f, 0.5f, 0.0f, 0.621f, 0.559f, 0.699f,
		0.0f, 0.0f, 0.0f, 0.621f, 0.559f, 0.699f,

		0.5f, 0.5f, 0.5f, 0.621f, 0.559f, 0.699f,
		0.5f, 0.5f, 0.0f, 0.621f, 0.559f, 0.699f,
		0.0f, 0.5f, 0.0f, 0.621f, 0.559f, 0.699f,
		0.0f, 0.5f, 0.5f, 0.621f, 0.559f, 0.699f,

		0.0f, 0.0f, 0.0f, 0.621f, 0.559f, 0.699f,
		0.5f, 0.0f, 0.0f, 0.621f, 0.559f, 0.699f,
		0.5f, 0.0f, 0.5f, 0.621f, 0.559f, 0.699f,
		0.0f, 0.0f, 0.5f, 0.621f, 0.559f, 0.699f
	};

	//left_right wall
	float wall2[] = {
		0.0f, 0.0f, 0.0f, 0.656f, 0.613f, 0.7304f,
		0.5f, 0.0f, 0.0f, 0.656f, 0.613f, 0.7304f,
		0.5f, 0.5f, 0.0f, 0.656f, 0.613f, 0.7304f,
		0.0f, 0.5f, 0.0f, 0.656f, 0.613f, 0.7304f,

		0.5f, 0.0f, 0.0f, 0.656f, 0.613f, 0.7304f,
		0.5f, 0.5f, 0.0f, 0.656f, 0.613f, 0.7304f,
		0.5f, 0.0f, 0.5f, 0.656f, 0.613f, 0.7304f,
		0.5f, 0.5f, 0.5f, 0.656f, 0.613f, 0.7304f,

		0.0f, 0.0f, 0.5f, 0.656f, 0.613f, 0.7304f,
		0.5f, 0.0f, 0.5f,0.656f, 0.613f, 0.7304f,
		0.5f, 0.5f, 0.5f, 0.656f, 0.613f, 0.7304f,
		0.0f, 0.5f, 0.5f, 0.656f, 0.613f, 0.7304f,

		0.0f, 0.0f, 0.5f, 0.656f, 0.613f, 0.7304f,
		0.0f, 0.5f, 0.5f, 0.656f, 0.613f, 0.7304f,
		0.0f, 0.5f, 0.0f, 0.656f, 0.613f, 0.7304f,
		0.0f, 0.0f, 0.0f, 0.656f, 0.613f, 0.7304f,

		0.5f, 0.5f, 0.5f, 0.656f, 0.613f, 0.7304f,
		0.5f, 0.5f, 0.0f, 0.656f, 0.613f, 0.7304f,
		0.0f, 0.5f, 0.0f, 0.656f, 0.613f, 0.7304f,
		0.0f, 0.5f, 0.5f, 0.656f, 0.613f, 0.7304f,

		0.0f, 0.0f, 0.0f, 0.656f, 0.613f, 0.7304f,
		0.5f, 0.0f, 0.0f, 0.656f, 0.613f, 0.7304f,
		0.5f, 0.0f, 0.5f, 0.656f, 0.613f, 0.7304f,
		0.0f, 0.0f, 0.5f, 0.656f, 0.613f, 0.7304f
	};



	//0f, 0f, 0f,
	float blackboard[] = {
		0.0f, 0.0f, 0.0f, 0.918f, 0.914f, 0.895f,
		0.5f, 0.0f, 0.0f, 0.918f, 0.914f, 0.895f,
		0.5f, 0.5f, 0.0f, 0.918f, 0.914f, 0.895f,
		0.0f, 0.5f, 0.0f, 0.918f, 0.914f, 0.895f,

		0.5f, 0.0f, 0.0f, 0.918f, 0.914f, 0.895f,
		0.5f, 0.5f, 0.0f, 0.918f, 0.914f, 0.895f,
		0.5f, 0.0f, 0.5f, 0.918f, 0.914f, 0.895f,
		0.5f, 0.5f, 0.5f, 0.918f, 0.914f, 0.895f,

		0.0f, 0.0f, 0.5f, 0.918f, 0.914f, 0.895f,
		0.5f, 0.0f, 0.5f, 0.918f, 0.914f, 0.895f,
		0.5f, 0.5f, 0.5f, 0.918f, 0.914f, 0.895f,
		0.0f, 0.5f, 0.5f, 0.918f, 0.914f, 0.895f,

		0.0f, 0.0f, 0.5f, 0.918f, 0.914f, 0.895f,
		0.0f, 0.5f, 0.5f, 0.918f, 0.914f, 0.895f,
		0.0f, 0.5f, 0.0f, 0.918f, 0.914f, 0.895f,
		0.0f, 0.0f, 0.0f, 0.918f, 0.914f, 0.895f,

		0.5f, 0.5f, 0.5f, 0.918f, 0.914f, 0.895f,
		0.5f, 0.5f, 0.0f, 0.918f, 0.914f, 0.895f,
		0.0f, 0.5f, 0.0f, 0.918f, 0.914f, 0.895f,
		0.0f, 0.5f, 0.5f, 0.918f, 0.914f, 0.895f,

		0.0f, 0.0f, 0.0f, 0.918f, 0.914f, 0.895f,
		0.5f, 0.0f, 0.0f, 0.918f, 0.914f, 0.895f,
		0.5f, 0.0f, 0.5f, 0.918f, 0.914f, 0.895f,
		0.0f, 0.0f, 0.5f, 0.918f, 0.914f, 0.895f
	};

	//0.29f, 0.0f, 0.29f,
	float cabinate[] = {
		0.0f, 0.0f, 0.0f,     0.93f, 0.305f, 0.203f,
		0.5f, 0.0f, 0.0f,     0.93f, 0.305f, 0.203f,
		0.5f, 0.5f, 0.0f,     0.93f, 0.305f, 0.203f,
		0.0f, 0.5f, 0.0f,     0.93f, 0.305f, 0.203f,

		0.5f, 0.0f, 0.0f,     0.93f, 0.305f, 0.203f,
		0.5f, 0.5f, 0.0f,     0.93f, 0.305f, 0.203f,
		0.5f, 0.0f, 0.5f,     0.93f, 0.305f, 0.203f,
		0.5f, 0.5f, 0.5f,    0.93f, 0.305f, 0.203f,

		0.0f, 0.0f, 0.5f,     0.93f, 0.305f, 0.203f,
		0.5f, 0.0f, 0.5f,     0.93f, 0.305f, 0.203f,
		0.5f, 0.5f, 0.5f,    0.93f, 0.305f, 0.203f,
		0.0f, 0.5f, 0.5f,     0.93f, 0.305f, 0.203f,

		0.0f, 0.0f, 0.5f,     0.93f, 0.305f, 0.203f,
		0.0f, 0.5f, 0.5f,    0.93f, 0.305f, 0.203f,
		0.0f, 0.5f, 0.0f,     0.93f, 0.305f, 0.203f,
		0.0f, 0.0f, 0.0f,     0.93f, 0.305f, 0.203f,

		0.5f, 0.5f, 0.5f,     0.93f, 0.305f, 0.203f,
		0.5f, 0.5f, 0.0f,     0.93f, 0.305f, 0.203f,
		0.0f, 0.5f, 0.0f,     0.93f, 0.305f, 0.203f,
		0.0f, 0.5f, 0.5f,     0.93f, 0.305f, 0.203f,

		0.0f, 0.0f, 0.0f,     0.93f, 0.305f, 0.203f,
		0.5f, 0.0f, 0.0f,    0.93f, 0.305f, 0.203f,
		0.5f, 0.0f, 0.5f,     0.93f, 0.305f, 0.203f,
		0.0f, 0.0f, 0.5f,     0.93f, 0.305f, 0.203f,
	};
	//0.95f, 0.95f, 0.95f,
	float ceiling[] = {
		0.0f, 0.0f, 0.0f, 0.95f, 0.95f, 0.95f,
		0.5f, 0.0f, 0.0f, 0.95f, 0.95f, 0.95f,
		0.5f, 0.5f, 0.0f, 0.95f, 0.95f, 0.95f,
		0.0f, 0.5f, 0.0f, 0.95f, 0.95f, 0.95f,

		0.5f, 0.0f, 0.0f, 0.95f, 0.95f, 0.95f,
		0.5f, 0.5f, 0.0f, 0.95f, 0.95f, 0.95f,
		0.5f, 0.0f, 0.5f, 0.95f, 0.95f, 0.95f,
		0.5f, 0.5f, 0.5f, 0.95f, 0.95f, 0.95f,

		0.0f, 0.0f, 0.5f, 0.95f, 0.95f, 0.95f,
		0.5f, 0.0f, 0.5f, 0.95f, 0.95f, 0.95f,
		0.5f, 0.5f, 0.5f, 0.95f, 0.95f, 0.95f,
		0.0f, 0.5f, 0.5f, 0.95f, 0.95f, 0.95f,

		0.0f, 0.0f, 0.5f, 0.95f, 0.95f, 0.95f,
		0.0f, 0.5f, 0.5f, 0.95f, 0.95f, 0.95f,
		0.0f, 0.5f, 0.0f, 0.95f, 0.95f, 0.95f,
		0.0f, 0.0f, 0.0f, 0.95f, 0.95f, 0.95f,

		0.5f, 0.5f, 0.5f, 0.95f, 0.95f, 0.95f,
		0.5f, 0.5f, 0.0f, 0.95f, 0.95f, 0.95f,
		0.0f, 0.5f, 0.0f, 0.95f, 0.95f, 0.95f,
		0.0f, 0.5f, 0.5f, 0.95f, 0.95f, 0.95f,

		0.0f, 0.0f, 0.0f, 0.95f, 0.95f, 0.95f,
		0.5f, 0.0f, 0.0f, 0.95f, 0.95f, 0.95f,
		0.5f, 0.0f, 0.5f, 0.95f, 0.95f, 0.95f,
		0.0f, 0.0f, 0.5f, 0.95f, 0.95f, 0.95f
	};

	//1.0f, 1.0f, 1.0f,
	float fan_holder[] = {
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,

		0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f
	};
	//.44f, .22f, .05f,
	float fan_pivot[] = {
		0.0f, 0.0f, 0.0f,    .1569f, .2f, .2902f,
		0.5f, 0.0f, 0.0f,    .1569f, .2f, .2902f,
		0.5f, 0.5f, 0.0f,     .1569f, .2f, .2902f,
		0.0f, 0.5f, 0.0f,    .1569f, .2f, .2902f,

		0.5f, 0.0f, 0.0f,    .1569f, .2f, .2902f,
		0.5f, 0.5f, 0.0f,    .1569f, .2f, .2902f,
		0.5f, 0.0f, 0.5f,    .1569f, .2f, .2902f,
		0.5f, 0.5f, 0.5f,    .1569f, .2f, .2902f,

		0.0f, 0.0f, 0.5f,     .1569f, .2f, .2902f,
		0.5f, 0.0f, 0.5f,    .1569f, .2f, .2902f,
		0.5f, 0.5f, 0.5f,    .1569f, .2f, .2902f,
		0.0f, 0.5f, 0.5f,    .1569f, .2f, .2902f,

		0.0f, 0.0f, 0.5f,     .1569f, .2f, .2902f,
		0.0f, 0.5f, 0.5f,     .1569f, .2f, .2902f,
		0.0f, 0.5f, 0.0f,    .1569f, .2f, .2902f,
		0.0f, 0.0f, 0.0f,     .1569f, .2f, .2902f,

		0.5f, 0.5f, 0.5f,     .1569f, .2f, .2902f,
		0.5f, 0.5f, 0.0f,     .1569f, .2f, .2902f,
		0.0f, 0.5f, 0.0f,    .1569f, .2f, .2902f,
		0.0f, 0.5f, 0.5f,     .1569f, .2f, .2902f,

		0.0f, 0.0f, 0.0f,     .1569f, .2f, .2902f,
		0.5f, 0.0f, 0.0f,     .1569f, .2f, .2902f,
		0.5f, 0.0f, 0.5f,    .1569f, .2f, .2902f,
		0.0f, 0.0f, 0.5f,    .1569f, .2f, .2902f,
	};
	//.0f, .0f, .42f,
	float fan_blade[] = {
		0.0f, 0.0f, 0.0f,   .49f, .7058f, .424f,
		0.5f, 0.0f, 0.0f,    .49f, .7058f, .424f,
		0.5f, 0.5f, 0.0f,    .49f, .7058f, .424f,
		0.0f, 0.5f, 0.0f,    .49f, .7058f, .424f,

		0.5f, 0.0f, 0.0f,    .49f, .7058f, .424f,
		0.5f, 0.5f, 0.0f,    .49f, .7058f, .424f,
		0.5f, 0.0f, 0.5f,    .49f, .7058f, .424f,
		0.5f, 0.5f, 0.5f,    .49f, .7058f, .424f,

		0.0f, 0.0f, 0.5f,    .49f, .7058f, .424f,
		0.5f, 0.0f, 0.5f,   .49f, .7058f, .424f,
		0.5f, 0.5f, 0.5f,    .49f, .7058f, .424f,
		0.0f, 0.5f, 0.5f,    .49f, .7058f, .424f,

		0.0f, 0.0f, 0.5f,    .49f, .7058f, .424f,
		0.0f, 0.5f, 0.5f,    .49f, .7058f, .424f,
		0.0f, 0.5f, 0.0f,    .49f, .7058f, .424f,
		0.0f, 0.0f, 0.0f,    .49f, .7058f, .424f,

		0.5f, 0.5f, 0.5f,    .49f, .7058f, .424f,
		0.5f, 0.5f, 0.0f,    .49f, .7058f, .424f,
		0.0f, 0.5f, 0.0f,    .49f, .7058f, .424f,
		0.0f, 0.5f, 0.5f,    .49f, .7058f, .424f,

		0.0f, 0.0f, 0.0f,    .49f, .7058f, .424f,
		0.5f, 0.0f, 0.0f,    .49f, .7058f, .424f,
		0.5f, 0.0f, 0.5f,    .49f, .7058f, .424f,
		0.0f, 0.0f, 0.5f,    .49f, .7058f, .424f,
	};
	
	float border[] = {
		0.0f, 0.0f, 0.0f, .0f, .0f, .0f,
		0.5f, 0.0f, 0.0f, .0f, .0f, .0f,
		0.5f, 0.5f, 0.0f, .0f, .0f, .0f,
		0.0f, 0.5f, 0.0f, .0f, .0f, .0f,

		0.5f, 0.0f, 0.0f, .0f, .0f, .0f,
		0.5f, 0.5f, 0.0f, .0f, .0f, .0f,
		0.5f, 0.0f, 0.5f, .0f, .0f, .0f,
		0.5f, 0.5f, 0.5f, .0f, .0f, .0f,

		0.0f, 0.0f, 0.5f, .0f, .0f, .0f,
		0.5f, 0.0f, 0.5f, .0f, .0f, .0f,
		0.5f, 0.5f, 0.5f, .0f, .0f, .0f,
		0.0f, 0.5f, 0.5f, .0f, .0f, .0f,

		0.0f, 0.0f, 0.5f, .0f, .0f, .0f,
		0.0f, 0.5f, 0.5f, .0f, .0f, .0f,
		0.0f, 0.5f, 0.0f, .0f, .0f, .0f,
		0.0f, 0.0f, 0.0f, .0f, .0f, .0f,

		0.5f, 0.5f, 0.5f, .0f, .0f, .0f,
		0.5f, 0.5f, 0.0f, .0f, .0f, .0f,
		0.0f, 0.5f, 0.0f, .0f, .0f, .0f,
		0.0f, 0.5f, 0.5f, .0f, .0f, .0f,

		0.0f, 0.0f, 0.0f, .0f, .0f, .0f,
		0.5f, 0.0f, 0.0f, .0f, .0f, .0f,
		0.5f, 0.0f, 0.5f, .0f, .0f, .0f,
		0.0f, 0.0f, 0.5f, .0f, .0f, .0f
	};
	unsigned int cube_indices[] = {
		0, 3, 2,
		2, 1, 0,

		4, 5, 7,
		7, 6, 4,

		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20
	};

	//for cylinder
	float decagon[] = {
	0.0f, 0.0f, 0.0f,  0.992f, 0.8588f, 0.1529f, // Center point
	-1.0f, 0.0f, -3.0f,  0.992f, 0.8588f, 0.1529f, // RED
	1.0f, 0.0f, -3.0f,    0.992f, 0.8588f, 0.1529f,// GREEN
	2.25f, 0.0f, -1.75f,     0.992f, 0.8588f, 0.1529f, // BLUE
	3.0f, 0.0f, 0.0f,    0.992f, 0.8588f, 0.1529f, // WHITE
	2.25f, 0.0f, 1.75f,    0.992f, 0.8588f, 0.1529f, // PINK
	1.0f, 0.0f, 3.0f,     0.992f, 0.8588f, 0.1529f, // KHOYERI
	-1.0f, 0.0f, 3.0f,     0.992f, 0.8588f, 0.1529f, // SKY BLUE
	-2.25f, 0.0f, 1.75f,     0.992f, 0.8588f, 0.1529f, // YELLOW
	-3.0f, 0.0f, 0.0f,    0.992f, 0.8588f, 0.1529f, // purple
	-2.25f, 0.0f, -1.75f,     0.992f, 0.8588f, 0.1529f, // orange

	0.0f, 1.0f, 0.0f,    0.0f, 0.694f, 0.824f, // Center point
	-1.0f, 1.0f, -3.0f,    0.0f, 0.694f, 0.824f, // RED
	1.0f, 1.0f, -3.0f,      0.0f, 0.694f, 0.824f, // GREEN
	2.25f, 1.0f, -1.75f,    0.0f, 0.694f, 0.824f,// BLUE
	3.0f, 1.0f, 0.0f,     0.0f, 0.694f, 0.824f, // WHITE
	2.25f, 1.0f, 1.75f,    0.0f, 0.694f, 0.824f, // PINK
	1.0f, 1.0f, 3.0f,     0.0f, 0.694f, 0.824f, // KHOYERI
	-1.0f, 1.0f, 3.0f,     0.0f, 0.694f, 0.824f, // SKY BLUE
	-2.25f, 1.0f, 1.75f,     0.0f, 0.694f, 0.824f,// YELLOW
	-3.0f, 1.0f, 0.0f,     0.0f, 0.694f, 0.824f, // purple
	-2.25f, 1.0f, -1.75f,     0.0f, 0.694f, 0.824f,// orange
	};
	unsigned int deca_indices[] = {
		0,1,2,
		0,2,3,
		0,3,4,
		0,4,5,
		0,5,6,
		0,6,7,
		0,7,8,
		0,8,9,
		0,9,10,
		0,10,1,

		11,12,13,
		11,13,14,
		11,14,15,
		11,15,16,
		11,16,17,
		11,17,18,
		11,18,19,
		11,19,20,
		11,20,21,
		11,21,12,


		2,12,13,
		2,12,1,

		3,13,14,
		3,13,2,

		4,14,15,
		4,14,3,

		5,15,16,
		5,15,4,

		6,16,17,
		6,16,5,

		7,17,18,
		7,17,6,

		8,18,19,
		8,18,7,

		9,19,20,
		9,19,8,

		10,20,21,
		10,20,9,

		1,21,12,
		1,21,10
	};
	

	//for glass
	float decagon2[] = {
		//0
	0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f, // Center point
	-1.0f, 0.0f, -3.0f, 1.0f, 0.5f, 0.0f, // RED
	1.0f, 0.0f, -3.0f,  1.0f, 0.5f, 0.0f, // GREEN
	2.25f, 0.0f, -1.75f, 1.0f, 0.5f, 0.0f, // BLUE
	3.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f, // WHITE
	2.25f, 0.0f, 1.75f,1.0f, 0.5f, 0.0f, // PINK
	1.0f, 0.0f, 3.0f, 1.0f, 0.5f, 0.0f, // KHOYERI
	-1.0f, 0.0f, 3.0f, 1.0f, 0.5f, 0.0f, // SKY BLUE
	-2.25f, 0.0f, 1.75f, 1.0f, 0.5f, 0.0f, // YELLOW
	-3.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f, // purple
	-2.25f, 0.0f, -1.75f, 1.0f, 0.5f, 0.0f, // orange
	//11
	0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Center point
	-1.0f, 1.0f, -3.0f,  1.0f, 1.0f, 1.0f,  // RED
	1.0f, 1.0f, -3.0f,   1.0f, 1.0f, 1.0f,  // GREEN
	2.25f, 1.0f, -1.75f, 1.0f, 1.0f, 1.0f,  // BLUE
	3.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  // WHITE
	2.25f, 1.0f, 1.75f,  1.0f, 1.0f, 1.0f,  // PINK
	1.0f, 1.0f, 3.0f,  1.0f, 1.0f, 1.0f,  // KHOYERI
	-1.0f, 1.0f, 3.0f, 1.0f, 1.0f, 1.0f,  // SKY BLUE
	-2.25f, 1.0f, 1.75f, 1.0f, 1.0f, 1.0f,  // YELLOW
	-3.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  // purple
	-2.25f, 1.0f, -1.75f,  1.0f, 1.0f, 1.0f,  // orange
	//22
	-4.0f, 0.0f, -4.0f, 1.0f, 1.0f, 1.0f,
	4.0f, 0.0f, -4.0f,  1.0f, 1.0f, 1.0f,
	4.0f, 0.0f, 4.0f, 1.0f, 1.0f, 1.0f,
	-4.0f, 0.0f, 4.0f, 1.0f, 1.0f, 1.0f, // WHITE
	};
	unsigned int deca2_indices[] = {
		0,1,2,
		0,2,3,
		0,3,4,
		0,4,5,
		0,5,6,
		0,6,7,
		0,7,8,
		0,8,9,
		0,9,10,
		0,10,1,

		/*11,12,13,
		11,13,14,
		11,14,15,
		11,15,16,
		11,16,17,
		11,17,18,
		11,18,19,
		11,19,20,
		11,20,21,
		11,21,12,*/


		2,12,13,
		2,12,1,

		3,13,14,
		3,13,2,

		4,14,15,
		4,14,3,

		5,15,16,
		5,15,4,

		6,16,17,
		6,16,5,

		7,17,18,
		7,17,6,

		8,18,19,
		8,18,7,

		9,19,20,
		9,19,8,

		10,20,21,
		10,20,9,

		1,21,12,
		1,21,10,

		///
		24,22,23,
		24,22,25

	};

	//for bottle
	float decagon3[] = {
		//0
	0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f, // Center point
	-1.0f, 0.0f, -3.0f, 1.0f, 0.5f, 0.0f, // RED
	1.0f, 0.0f, -3.0f,  1.0f, 0.5f, 0.0f, // GREEN
	2.25f, 0.0f, -1.75f, 1.0f, 0.5f, 0.0f, // BLUE
	3.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f, // WHITE
	2.25f, 0.0f, 1.75f,1.0f, 0.5f, 0.0f, // PINK
	1.0f, 0.0f, 3.0f, 1.0f, 0.5f, 0.0f, // KHOYERI
	-1.0f, 0.0f, 3.0f, 1.0f, 0.5f, 0.0f, // SKY BLUE
	-2.25f, 0.0f, 1.75f, 1.0f, 0.5f, 0.0f, // YELLOW
	-3.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f, // purple
	-2.25f, 0.0f, -1.75f, 1.0f, 0.5f, 0.0f, // orange
	//11
	0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Center point
	-1.0f, 1.0f, -3.0f,  1.0f, 1.0f, 1.0f,  // RED
	1.0f, 1.0f, -3.0f,   1.0f, 1.0f, 1.0f,  // GREEN
	2.25f, 1.0f, -1.75f, 1.0f, 1.0f, 1.0f,  // BLUE
	3.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  // WHITE
	2.25f, 1.0f, 1.75f,  1.0f, 1.0f, 1.0f,  // PINK
	1.0f, 1.0f, 3.0f,  1.0f, 1.0f, 1.0f,  // KHOYERI
	-1.0f, 1.0f, 3.0f, 1.0f, 1.0f, 1.0f,  // SKY BLUE
	-2.25f, 1.0f, 1.75f, 1.0f, 1.0f, 1.0f,  // YELLOW
	-3.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  // purple
	-2.25f, 1.0f, -1.75f,  1.0f, 1.0f, 1.0f,  // orange
	//22
	-1.0f, 1.9f, -1.0f, .0f, .0f, .0f,
	1.0f, 1.9f, -1.0f,  .0f, .0f, .0f,
	1.0f, 1.9f, 1.0f, .0f, .0f, .0f,
	-1.0f, 1.9f, 1.0f, .0f, .0f, .0f, // WHITE
	};
	unsigned int deca3_indices[] = {
		0,1,2,
		0,2,3,
		0,3,4,
		0,4,5,
		0,5,6,
		0,6,7,
		0,7,8,
		0,8,9,
		0,9,10,
		0,10,1,

		/*11,12,13,
		11,13,14,
		11,14,15,
		11,15,16,
		11,16,17,
		11,17,18,
		11,18,19,
		11,19,20,
		11,20,21,
		11,21,12,*/

		2,12,13,
		2,12,1,
		3,13,14,
		3,13,2,
		4,14,15,
		4,14,3,
		5,15,16,
		5,15,4,
		6,16,17,
		6,16,5,
		7,17,18,
		7,17,6,
		8,18,19,
		8,18,7,
		9,19,20,
		9,19,8,
		10,20,21,
		10,20,9,
		1,21,12,
		1,21,10,

		24,22,23,
		24,22,25,

		23,12,13,
		23,12,22,
		23,13,14,
		22,21,12,
		22,20,21,
		23,14,15,
		22,25,20,
		24,23,15,
		24,15,16,
		25,19,20,
		24,16,17,
		25,18,19,
		25,17,18,
		25,17,24,
		

	};

	//for bottle2
	float decagon4[] = {
	//0
	0.0f, 0.0f, 0.0f, .0f, 1.0f, 1.0f, // Center point
	-1.0f, 0.0f, -3.0f,.0f, 1.0f, 1.0f, // RED
	1.0f, 0.0f, -3.0f,  .0f, 1.0f, 1.0f, // GREEN
	2.25f, 0.0f, -1.75f, .0f, 1.0f, 1.0f, // BLUE
	3.0f, 0.0f, 0.0f, .0f, 1.0f, 1.0f, // WHITE
	2.25f, 0.0f, 1.75f, .0f, 1.0f, 1.0f, // PINK
	1.0f, 0.0f, 3.0f, .0f, 1.0f, 1.0f, // KHOYERI
	-1.0f, 0.0f, 3.0f, .0f, 1.0f, 1.0f, // SKY BLUE
	-2.25f, 0.0f, 1.75f, .0f, 1.0f, 1.0f, // YELLOW
	-3.0f, 0.0f, 0.0f, .0f, 1.0f, 1.0f, // purple
	-2.25f, 0.0f, -1.75f, .0f, 1.0f, 1.0f, // orange
	//11
	0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Center point
	-1.0f, 1.0f, -3.0f,  1.0f, 1.0f, 1.0f,  // RED
	1.0f, 1.0f, -3.0f,   1.0f, 1.0f, 1.0f,  // GREEN
	2.25f, 1.0f, -1.75f, 1.0f, 1.0f, 1.0f,  // BLUE
	3.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  // WHITE
	2.25f, 1.0f, 1.75f,  1.0f, 1.0f, 1.0f,  // PINK
	1.0f, 1.0f, 3.0f,  1.0f, 1.0f, 1.0f,  // KHOYERI
	-1.0f, 1.0f, 3.0f, 1.0f, 1.0f, 1.0f,  // SKY BLUE
	-2.25f, 1.0f, 1.75f, 1.0f, 1.0f, 1.0f,  // YELLOW
	-3.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  // purple
	-2.25f, 1.0f, -1.75f,  1.0f, 1.0f, 1.0f,  // orange
	//22
	-1.0f, 1.9f, -1.0f, 1.0f, .5f, .0f,
	1.0f, 1.9f, -1.0f,  1.0f, .5f, .0f,
	1.0f, 1.9f, 1.0f, 1.0f, .5f, .0f,
	-1.0f, 1.9f, 1.0f, 1.0f, .5f, .0f, // WHITE
	};
	unsigned int deca4_indices[] = {
		0,1,2,
		0,2,3,
		0,3,4,
		0,4,5,
		0,5,6,
		0,6,7,
		0,7,8,
		0,8,9,
		0,9,10,
		0,10,1,

		/*11,12,13,
		11,13,14,
		11,14,15,
		11,15,16,
		11,16,17,
		11,17,18,
		11,18,19,
		11,19,20,
		11,20,21,
		11,21,12,*/

		2,12,13,
		2,12,1,
		3,13,14,
		3,13,2,
		4,14,15,
		4,14,3,
		5,15,16,
		5,15,4,
		6,16,17,
		6,16,5,
		7,17,18,
		7,17,6,
		8,18,19,
		8,18,7,
		9,19,20,
		9,19,8,
		10,20,21,
		10,20,9,
		1,21,12,
		1,21,10,

		24,22,23,
		24,22,25,

		23,12,13,
		23,12,22,
		23,13,14,
		22,21,12,
		22,20,21,
		23,14,15,
		22,25,20,
		24,23,15,
		24,15,16,
		25,19,20,
		24,16,17,
		25,18,19,
		25,17,18,
		25,17,24,


	};

	//for bottle3
	float decagon5[] = {
		//0
		0.0f, 0.0f, 0.0f,    1.0f, .5f, .3125f, // Center point
		-1.0f, 0.0f, -3.0f,  1.0f, .5f, .3125f, // RED
		1.0f, 0.0f, -3.0f,   1.0f, .5f, .3125f,  // GREEN
		2.25f, 0.0f, -1.75f,   1.0f, .5f, .3125f, // BLUE
		3.0f, 0.0f, 0.0f,        1.0f, .5f, .3125f, // WHITE
		2.25f, 0.0f, 1.75f,   1.0f, .5f, .3125f, // PINK
		1.0f, 0.0f, 3.0f,    1.0f, .5f, .3125f,// KHOYERI
		-1.0f, 0.0f, 3.0f,    1.0f, .5f, .3125f,// SKY BLUE
		-2.25f, 0.0f, 1.75f,    1.0f, .5f, .3125f,// YELLOW
		-3.0f, 0.0f, 0.0f,    1.0f, .5f, .3125f,// purple
		-2.25f, 0.0f, -1.75f,    1.0f, .5f, .3125f,// orange
		//11
		0.0f, 1.0f, 0.0f,     .469f, .222f, .215f, // Center point
		-1.0f, 1.0f, -3.0f,      .469f, .222f, .215f,  // RED
		1.0f, 1.0f, -3.0f,       .469f, .222f, .215f,  // GREEN
		2.25f, 1.0f, -1.75f,     .469f, .222f, .215f,  // BLUE
		3.0f, 1.0f, 0.0f,     .469f, .222f, .215f, // WHITE
		2.25f, 1.0f, 1.75f,     .469f, .222f, .215f,  // PINK
		1.0f, 1.0f, 3.0f,     .469f, .222f, .215f,  // KHOYERI
		-1.0f, 1.0f, 3.0f,     .469f, .222f, .215f,  // SKY BLUE
		-2.25f, 1.0f, 1.75f,    .469f, .222f, .215f, // YELLOW
		-3.0f, 1.0f, 0.0f,      .469f, .222f, .215f,// purple
		-2.25f, 1.0f, -1.75f,     .469f, .222f, .215f,  // orange
		//22
		-1.0f, 1.9f, -1.0f,    1.0f, .5f, .0f,
		1.0f, 1.9f, -1.0f,  1.0f, .5f, .0f,
		1.0f, 1.9f, 1.0f, 1.0f, .5f, .0f,
		-1.0f, 1.9f, 1.0f, 1.0f, .5f, .0f, // WHITE
	};
	unsigned int deca5_indices[] = {
		0,1,2,
		0,2,3,
		0,3,4,
		0,4,5,
		0,5,6,
		0,6,7,
		0,7,8,
		0,8,9,
		0,9,10,
		0,10,1,

		/*11,12,13,
		11,13,14,
		11,14,15,
		11,15,16,
		11,16,17,
		11,17,18,
		11,18,19,
		11,19,20,
		11,20,21,
		11,21,12,*/

		2,12,13,
		2,12,1,
		3,13,14,
		3,13,2,
		4,14,15,
		4,14,3,
		5,15,16,
		5,15,4,
		6,16,17,
		6,16,5,
		7,17,18,
		7,17,6,
		8,18,19,
		8,18,7,
		9,19,20,
		9,19,8,
		10,20,21,
		10,20,9,
		1,21,12,
		1,21,10,

		24,22,23,
		24,22,25,

		23,12,13,
		23,12,22,
		23,13,14,
		22,21,12,
		22,20,21,
		23,14,15,
		22,25,20,
		24,23,15,
		24,15,16,
		25,19,20,
		24,16,17,
		25,18,19,
		25,17,18,
		25,17,24,


	};

	//for bulb
	float decagon6[] = {
		//0
		0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f, // Center point
		-1.0f, 0.0f, -3.0f,  1.0f, 1.0f, 1.0f,  // RED
		1.0f, 0.0f, -3.0f,  1.0f, 1.0f, 1.0f,   // GREEN
		2.25f, 0.0f, -1.75f,  1.0f, 1.0f, 1.0f,  // BLUE
		3.0f, 0.0f, 0.0f,       1.0f, 1.0f, 1.0f,  // WHITE
		2.25f, 0.0f, 1.75f,   1.0f, 1.0f, 1.0f, // PINK
		1.0f, 0.0f, 3.0f,   1.0f, 1.0f, 1.0f, // KHOYERI
		-1.0f, 0.0f, 3.0f,    1.0f, 1.0f, 1.0f, // SKY BLUE
		-2.25f, 0.0f, 1.75f,    1.0f, 1.0f, 1.0f, // YELLOW
		-3.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f, // purple
		-2.25f, 0.0f, -1.75f,    1.0f, 1.0f, 1.0f, // orange
		//11
		0.0f, .1f, 0.0f,     1.0f, 1.0f, 1.0f, // Center point
		-1.0f, .1f, -3.0f,      1.0f, 1.0f, 1.0f,   // RED
		1.0f, .1f, -3.0f,       1.0f, 1.0f, 1.0f,   // GREEN
		2.25f, .1f, -1.75f,     1.0f, 1.0f, 1.0f,  // BLUE
		3.0f, .1f, 0.0f,    1.0f, 1.0f, 1.0f,  // WHITE
		2.25f, .1f, 1.75f,     1.0f, 1.0f, 1.0f,   // PINK
		1.0f, .1f, 3.0f,     1.0f, 1.0f, 1.0f,   // KHOYERI
		-1.0f, .1f, 3.0f,    1.0f, 1.0f, 1.0f,  // SKY BLUE
		-2.25f, .1f, 1.75f,    1.0f, 1.0f, 1.0f, // YELLOW
		-3.0f, .1f, 0.0f,      1.0f, 1.0f, 1.0f, // purple
		-2.25f, .1f, -1.75f,    1.0f, 1.0f, 1.0f,// orange
		//22
		-1.0f, 1.9f, -1.0f,    .109f, .109f, .1054f,
		1.0f, 1.9f, -1.0f, .109f, .109f, .1054f,
		1.0f, 1.9f, 1.0f,  .109f, .109f, .1054f,
		-1.0f, 1.9f, 1.0f, .109f, .109f, .1054f,// WHITE
	};
	unsigned int deca6_indices[] = {
		0,1,2,
		0,2,3,
		0,3,4,
		0,4,5,
		0,5,6,
		0,6,7,
		0,7,8,
		0,8,9,
		0,9,10,
		0,10,1,

		/*11,12,13,
		11,13,14,
		11,14,15,
		11,15,16,
		11,16,17,
		11,17,18,
		11,18,19,
		11,19,20,
		11,20,21,
		11,21,12,*/

		2,12,13,
		2,12,1,
		3,13,14,
		3,13,2,
		4,14,15,
		4,14,3,
		5,15,16,
		5,15,4,
		6,16,17,
		6,16,5,
		7,17,18,
		7,17,6,
		8,18,19,
		8,18,7,
		9,19,20,
		9,19,8,
		10,20,21,
		10,20,9,
		1,21,12,
		1,21,10,

		24,22,23,
		24,22,25,

		23,12,13,
		23,12,22,
		23,13,14,
		22,21,12,
		22,20,21,
		23,14,15,
		22,25,20,
		24,23,15,
		24,15,16,
		25,19,20,
		24,16,17,
		25,18,19,
		25,17,18,
		25,17,24,


	};


	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(table_top), table_top, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	unsigned int VBO2, VAO2, EBO2;
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(table_leg), table_leg, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);


	unsigned int VBO3, VAO3, EBO3;
	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	glGenBuffers(1, &EBO3);
	glBindVertexArray(VAO3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(chair_leg), chair_leg, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);


	unsigned int VBO4, VAO4, EBO4;
	glGenVertexArrays(1, &VAO4);
	glGenBuffers(1, &VBO4);
	glGenBuffers(1, &EBO4);
	glBindVertexArray(VAO4);
	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(chair_pillar), chair_pillar, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);


	unsigned int VBO5, VAO5, EBO5;
	glGenVertexArrays(1, &VAO5);
	glGenBuffers(1, &VBO5);
	glGenBuffers(1, &EBO5);
	glBindVertexArray(VAO5);
	glBindBuffer(GL_ARRAY_BUFFER, VBO5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(chair_back), chair_back, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO5);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);


	unsigned int VBOG, VAOG, EBOG;
	glGenVertexArrays(1, &VAOG);
	glGenBuffers(1, &VBOG);
	glGenBuffers(1, &EBOG);
	glBindVertexArray(VAOG);
	glBindBuffer(GL_ARRAY_BUFFER, VBOG);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOG);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);


	unsigned int VBOW1, VAOW1, EBOW1;
	glGenVertexArrays(1, &VAOW1);
	glGenBuffers(1, &VBOW1);
	glGenBuffers(1, &EBOW1);
	glBindVertexArray(VAOW1);
	glBindBuffer(GL_ARRAY_BUFFER, VBOW1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wall1), wall1, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOW1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);


	unsigned int VBOW2, VAOW2, EBOW2;
	glGenVertexArrays(1, &VAOW2);
	glGenBuffers(1, &VBOW2);
	glGenBuffers(1, &EBOW2);
	glBindVertexArray(VAOW2);
	glBindBuffer(GL_ARRAY_BUFFER, VBOW2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wall2), wall2, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOW2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);


	unsigned int VBOB, VAOB, EBOB;
	glGenVertexArrays(1, &VAOB);
	glGenBuffers(1, &VBOB);
	glGenBuffers(1, &EBOB);
	glBindVertexArray(VAOB);
	glBindBuffer(GL_ARRAY_BUFFER, VBOB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(blackboard), blackboard, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);


	unsigned int VBOC, VAOC, EBOC;
	glGenVertexArrays(1, &VAOC);
	glGenBuffers(1, &VBOC);
	glGenBuffers(1, &EBOC);
	glBindVertexArray(VAOC);
	glBindBuffer(GL_ARRAY_BUFFER, VBOC);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cabinate), cabinate, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOC);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);


	unsigned int VBOT, VAOT, EBOT;
	glGenVertexArrays(1, &VAOT);
	glGenBuffers(1, &VBOT);
	glGenBuffers(1, &EBOT);
	glBindVertexArray(VAOT);
	glBindBuffer(GL_ARRAY_BUFFER, VBOT);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ceiling), ceiling, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOT);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);


	//Fan
	unsigned int VBOF1, VAOF1, EBOF1;
	glGenVertexArrays(1, &VAOF1);
	glGenBuffers(1, &VBOF1);
	glGenBuffers(1, &EBOF1);
	glBindVertexArray(VAOF1);
	glBindBuffer(GL_ARRAY_BUFFER, VBOF1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fan_holder), fan_holder, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOF1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);


	unsigned int VBOF2, VAOF2, EBOF2;
	glGenVertexArrays(1, &VAOF2);
	glGenBuffers(1, &VBOF2);
	glGenBuffers(1, &EBOF2);
	glBindVertexArray(VAOF2);
	glBindBuffer(GL_ARRAY_BUFFER, VBOF2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fan_pivot), fan_pivot, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOF2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);


	unsigned int VBOF3, VAOF3, EBOF3;
	glGenVertexArrays(1, &VAOF3);
	glGenBuffers(1, &VBOF3);
	glGenBuffers(1, &EBOF3);
	glBindVertexArray(VAOF3);
	glBindBuffer(GL_ARRAY_BUFFER, VBOF3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fan_blade), fan_blade, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOF3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);


	//for decagon
	unsigned int VBOdec, VAOdec, EBOdec;
	glGenVertexArrays(1, &VAOdec);
	glGenBuffers(1, &VBOdec);
	glGenBuffers(1, &EBOdec);
	glBindVertexArray(VAOdec);
	glBindBuffer(GL_ARRAY_BUFFER, VBOdec);
	glBufferData(GL_ARRAY_BUFFER, sizeof(decagon), decagon, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOdec);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(deca_indices), deca_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	//for decagon2
	unsigned int VBOdec2, VAOdec2, EBOdec2;
	glGenVertexArrays(1, &VAOdec2);
	glGenBuffers(1, &VBOdec2);
	glGenBuffers(1, &EBOdec2);
	glBindVertexArray(VAOdec2);
	glBindBuffer(GL_ARRAY_BUFFER, VBOdec2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(decagon2), decagon2, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOdec2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(deca2_indices), deca2_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	//for decagon3
	unsigned int VBOdec3, VAOdec3, EBOdec3;
	glGenVertexArrays(1, &VAOdec3);
	glGenBuffers(1, &VBOdec3);
	glGenBuffers(1, &EBOdec3);
	glBindVertexArray(VAOdec3);
	glBindBuffer(GL_ARRAY_BUFFER, VBOdec3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(decagon3), decagon3, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOdec3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(deca3_indices), deca3_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	//for decagon4
	unsigned int VBOdec4, VAOdec4, EBOdec4;
	glGenVertexArrays(1, &VAOdec4);
	glGenBuffers(1, &VBOdec4);
	glGenBuffers(1, &EBOdec4);
	glBindVertexArray(VAOdec4);
	glBindBuffer(GL_ARRAY_BUFFER, VBOdec4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(decagon4), decagon4, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOdec4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(deca4_indices), deca4_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	//for decagon5
	unsigned int VBOdec5, VAOdec5, EBOdec5;
	glGenVertexArrays(1, &VAOdec5);
	glGenBuffers(1, &VBOdec5);
	glGenBuffers(1, &EBOdec5);
	glBindVertexArray(VAOdec5);
	glBindBuffer(GL_ARRAY_BUFFER, VBOdec5);
	glBufferData(GL_ARRAY_BUFFER, sizeof(decagon5), decagon5, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOdec5);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(deca5_indices), deca5_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);

	//for decagon6
	unsigned int VBOdec6, VAOdec6, EBOdec6;
	glGenVertexArrays(1, &VAOdec6);
	glGenBuffers(1, &VBOdec6);
	glGenBuffers(1, &EBOdec6);
	glBindVertexArray(VAOdec6);
	glBindBuffer(GL_ARRAY_BUFFER, VBOdec6);
	glBufferData(GL_ARRAY_BUFFER, sizeof(decagon6), decagon6, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOdec6);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(deca6_indices), deca6_indices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
	glEnableVertexAttribArray(1);


	int i = 0;
	Table_Chair table_chair[3];
	//stool stool[1];
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// activate shader
		ourShader.use();
		glm::mat4 model;
		// pass projection matrix to shader (note that in this case it could change every frame)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		//glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		// camera/view transformation
		float degree = 0;
		glm::mat4 view = camera.GetViewMatrix();
		float r = glm::length(camera.Position-glm::vec3(view[3]));
		//std::cout << "Vector: (" << camera.Position.x << ", " << camera.Position.y << ", " << camera.Position.z << ")" << std::endl;
		//std::cout << "Vector: (" << -glm::vec3(view[2]).x << ", " << -glm::vec3(view[2]).y << ", " << -glm::vec3(view[2]).z << ")" << std::endl;
		//glm::mat4 view = basic_camera.createViewMatrix();
		ourShader.setMat4("view", view);



		float shiftx = 5, shiftz = 0;
		for (int i = 0; i < 3; i++) {
			table_chair[i].tox = shiftx;
			table_chair[i].toz = shiftz;
			ourShader = table_chair[i].ret_shader(ourShader, VAO, VAO2, VAO3, VAO4, VAO5);
			shiftz -= 2.97;
		}
		//Table_Chair tc;
		//tc.tox = 5;
		//tc.toz = -8.5;
		//ourShader = tc.local_rotation(ourShader, VAO, VAO2, VAO3, VAO4, VAO5, 135);

		//Wall1
		model = transformation(-2.5, -.75, -9, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 20, 7, 0.2);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOW1);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		/*model = transformation(-2.5, -.75, 3, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 20, 7, 0.2);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOW1);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/


		//Wall2
		model = transformation(-2.5, -.75, -9, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, .2, 7, 24);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOW2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		model = transformation(7.5, -.75, -9, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, .2, 7, 24);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOW2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//right whiteBoard
		model = transformation(5, .7, -8.9, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 4.5, 2.5, 0.2);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOB);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		model = transformation(4.95, .65, -8.95, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 4.7, 2.7, 0.2);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO4);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//left whiteBoard
		model = transformation(-.3, .3, -8.9, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 3.3, 4, 0.2);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOB);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		model = transformation(-.35, 0.25, -8.95, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 3.5, 4.2, 0.2);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO4);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		//1st layer of biggg table
		model = transformation(-.3, -.75, -5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 3, .2, 16);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOF2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		model = transformation(-.3, -.659, -5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 3, 2, 16);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		model = transformation(-.3, .34, -5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 3, .2, 16);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO5);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//Cabinate
		model = transformation(-2.45, -.75, -8, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 1, 6, 4);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOC);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		model = transformation(-1.95, .6, -7.95, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, .01, 3.0, 1.8);
		ourShader.setMat4("model", model);//right
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		model = transformation(-1.95, .6, -6.95, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, .01, 3.0, 1.8);
		ourShader.setMat4("model", model);//left
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		model = transformation(-1.95, 0, -7.95, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, .01, 1.0, 3.8);
		ourShader.setMat4("model", model);//2ns_last_bottom
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		model = transformation(-1.95, -.6, -7.95, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, .01, 1.0, 3.8);
		ourShader.setMat4("model", model);//bottom
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//Ceiling
		/*model = transformation(-2.5, 2.75, -9, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 20, 0.1, 24);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO5);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/

		//Fan
		/*model = transformation(2, 2.5, -6, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 1, .5, 1);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOF1);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);*/
		model = transformation(2.125, 2.25, -5.875, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, .5, 1, .5);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAOF2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);



		//chessboard
		const float floorWidth = 10.0f;  // Width of the floor
		const float floorLength = 12.0f;  // Length of the floor
		const float tileWidth = 2.0f;  // Width of each tile
		const float tileLength = 2.0f;  // Length of each tile

		const float startX = -2.5f;
		const float startY = -0.8f;
		const float startZ = -9.0f;
		// Calculate the number of tiles in X and Z directions
		const int numTilesX = static_cast<int>(2 * floorWidth / tileWidth);
		const int numTilesZ = static_cast<int>(2 * floorLength / tileLength);
		// Loop to draw tiles on the floor
		for (int i = 0; i < numTilesX; ++i) {
			for (int j = 0; j < numTilesZ; ++j) {
				// Calculate the translation for each tile
				float tx = startX + i * .5f * tileWidth;
				float tz = startZ + j * .5f * tileLength;

				// Alternate between black and white tiles
				if ((i + j) % 2 == 0) {
					glBindVertexArray(VAO4); //black
				}
				else {
					glBindVertexArray(VAOB); //white
				}

				// Apply transformation for each tile
				model = transformation(tx, startY, tz, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, tileWidth - 0.01f, 0.1f, tileLength - 0.01f);
				ourShader.setMat4("model", model);

				// Draw the tile
				glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			}
		}


		//vending_maching
		model = transformation(2.4, -.75, -8.9, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 3, 2, 1);
		ourShader.setMat4("model", model);//base
		glBindVertexArray(VAOF2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		model = transformation(3.15, -0.05, -8.9, 90, 90, rotateAngle_Z, .251, .45, .249);
		ourShader.setMat4("model", model);//top
		glBindVertexArray(VAOdec);
		glDrawElements(GL_TRIANGLES, 120, GL_UNSIGNED_INT, 0);
		model = transformation(2.52, 0, -8.4, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.5, .1, .01);//.01, .2, 3.8
		ourShader.setMat4("model", model);//line
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		model = transformation(2.52, -.2, -8.4, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.5, .1, .01);//.01, .2, 3.8
		ourShader.setMat4("model", model);//line
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		model = transformation(2.52, -.4, -8.4, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 2.5, .1, .01);//.01, .2, 3.8
		ourShader.setMat4("model", model);//line
		glBindVertexArray(VAO2);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


		//bulb
		bulb(ourShader, VAOdec6, VAOdec, VAO2, -1, 0.8, 2);
		bulb(ourShader, VAOdec6, VAOdec, VAO2, -1, 0.8, 0);
		bulb(ourShader, VAOdec6, VAOdec, VAO2, -1, 0.8, -2);


		//bottle
		bottle(ourShader, VAOdec3, -2, 1.8, -.5, .035, .4, .035);
		bottle(ourShader, VAOdec3, -2, 1.8, 1.5, .035, .4, .035);
		bottle(ourShader, VAOdec3, -2, 1.8, -2.5, .035, .4, .035);
		bottle(ourShader, VAOdec4, -2, 1.8, 1, .035, .4, .035);
		bottle(ourShader, VAOdec4, -2, 1.8, -3, .035, .4, .035);
		bottle(ourShader, VAOdec4, -2, 1.8, -2, .035, .4, .035);
		bottle(ourShader, VAOdec5, -2, 1.8, 2.5, .035, .4, .035);
		bottle(ourShader, VAOdec5, -2, 1.8, 0, .035, .4, .035);

		bottle(ourShader, VAOdec3, -2, .8, 0, .035, .4, .035);
		bottle(ourShader, VAOdec3, -2, .8, 1, .035, .4, .035);
		bottle(ourShader, VAOdec3, -2, .8, -2, .035, .4, .035);
		bottle(ourShader, VAOdec4, -2, .8, -3, .035, .4, .035);
		bottle(ourShader, VAOdec4, -2, .8, 0, .035, .4, .035);
		bottle(ourShader, VAOdec4, -2, .8, 2.3, .035, .4, .035);
		bottle(ourShader, VAOdec5, -2, .8, -1, .035, .4, .035);


		//glass
		glass(ourShader, VAOdec2, .6, .45, 2, .025, .3, .025);
		glass(ourShader, VAOdec2, .6, .45, -1, .025, .3, .025);
		glass(ourShader, VAOdec2, 5.7, .105, -2, .025, .3, .025);
		glass(ourShader, VAOdec2, 5.9, .105, 1, .025, .3, .025);
		glass(ourShader, VAOdec2, 6, .105, -5, .025, .3, .025);


		//stool
		cylinder(ourShader, VAOdec, 2, -0.05, 2, .11, .2, .11);
		cylinder(ourShader, VAOdec, 2, -.75, 2, .01, .7, .01);
		cylinder(ourShader, VAOdec, 2, -.75, 2, .08, .01, .08);

		cylinder(ourShader, VAOdec, 2, -0.05, 0.5, .11, .2, .11);
		cylinder(ourShader, VAOdec, 2, -.75, 0.5, .01, .7, .01);
		cylinder(ourShader, VAOdec, 2, -.75, 0.5, .08, .01, .08);

		cylinder(ourShader, VAOdec, 2, -0.05, -1, .11, .2, .11);
		cylinder(ourShader, VAOdec, 2, -.75, -1, .01, .7, .01);
		cylinder(ourShader, VAOdec, 2, -.75, -1, .08, .01, .08);

		cylinder(ourShader, VAOdec, 2, -0.05, -2.5, .11, .2, .11);
		cylinder(ourShader, VAOdec, 2, -.75, -2.5, .01, .7, .01);
		cylinder(ourShader, VAOdec, 2, -.75, -2.5, .08, .01, .08);

		cylinder(ourShader, VAOdec, 2, -0.05, -4, .11, .2, .11);
		cylinder(ourShader, VAOdec, 2, -.75, -4, .01, .7, .01);
		cylinder(ourShader, VAOdec, 2, -.75, -4, .08, .01, .08);


		
		//rack
		model = transformation(-2.4, 0.75 , -3.5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 1.5, .2, 13);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO4);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); //bottom

		model = transformation(-2.4, 1.730, -3.5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 1.5, .2, 13);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO4);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);//middle

		model = transformation(-2.4, 2.65, -3.5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 1.5, .2, 13);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO4);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);//top

		model = transformation(-2.4, .8, -3.5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, .01, 3.97, 13);
		ourShader.setMat4("model", model);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);//background






\
		Fan fan;
		ourShader = fan.local_rotation(ourShader, VAOF3, i);
		if(fan_turn)
			i+=10;


		if(rotate_around)
			camera.ProcessKeyboard(Y_LEFT, deltaTime);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);
	glDeleteVertexArrays(1, &VAO3);
	glDeleteBuffers(1, &VBO3);
	glDeleteBuffers(1, &EBO3);
	glDeleteVertexArrays(1, &VAO4);
	glDeleteBuffers(1, &VBO4);
	glDeleteBuffers(1, &EBO4);
	glDeleteVertexArrays(1, &VAO5);
	glDeleteBuffers(1, &VBO5);
	glDeleteBuffers(1, &EBO5);
	glDeleteVertexArrays(1, &VAOG);
	glDeleteBuffers(1, &VBOG);
	glDeleteBuffers(1, &EBOG);
	glDeleteVertexArrays(1, &VAOW1);
	glDeleteBuffers(1, &VBOW1);
	glDeleteBuffers(1, &EBOW1);
	glDeleteVertexArrays(1, &VAOW2);
	glDeleteBuffers(1, &VBOW2);
	glDeleteBuffers(1, &EBOW2);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		camera.ProcessKeyboard(UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		camera.ProcessKeyboard(DOWN, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		camera.ProcessKeyboard(P_UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		camera.ProcessKeyboard(P_DOWN, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
		camera.ProcessKeyboard(Y_LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
		camera.ProcessKeyboard(Y_RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		camera.ProcessKeyboard(R_LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		camera.ProcessKeyboard(R_RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		if (!fan_turn) {
			fan_turn = true;
		}
		else {
			fan_turn = false;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		if (!rotate_around) {
			rotate_around = true;
		}
		else {
			rotate_around = false;
		}
	}

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

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
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
