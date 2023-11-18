#pragma once
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <learnopengl/filesystem.h>
#include "model.h"
#include "shader_s.h"

#include "player.h"
#include <iostream>
#include <gl/glut.h>
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glfw3.lib")

#define BITMAP_ID 0x4D42

GLint MenuInit(int menushaderProgram, const char* pathTexture) {
	const char* menuvertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec2 inTexcoord;\n"
		"out vec2 outTexcoord;\n"
		"void main()\n"
		"{\n"
		"   gl_Position =vec4(aPos,1.0);\n"
		"   outTexcoord = vec2(inTexcoord.x, inTexcoord.y);\n"
		"}\0";

	const char* menufragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec2 outTexcoord;\n"
		"uniform sampler2D menutexture;\n"
		"void main()\n"
		"{\n"
		"   FragColor = texture(menutexture, outTexcoord);\n"
		"}\n\0";

	float bgvertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	};


	int menuvertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(menuvertexShader, 1, &menuvertexShaderSource, NULL);
	glCompileShader(menuvertexShader);

	int menufragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(menufragmentShader, 1, &menufragmentShaderSource, NULL);
	glCompileShader(menufragmentShader);

	glAttachShader(menushaderProgram, menuvertexShader);
	glAttachShader(menushaderProgram, menufragmentShader);
	glLinkProgram(menushaderProgram);


	GLuint menutexture;
	glGenTextures(1, &menutexture);
	glBindTexture(GL_TEXTURE_2D, menutexture);

	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char* menudata = stbi_load(pathTexture, &width, &height, &nrChannels, 0);
	if (menudata)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, menudata);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	glUseProgram(menushaderProgram);
	glUniform1i(glGetUniformLocation(menushaderProgram, "menutexture"), 3);
	
	stbi_image_free(menudata);

	return menutexture;
}

void DrawMenu(GLuint menutexture, GLuint menushaderProgram, GLuint menuVAO, GLFWwindow* window) {

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glViewport(0, 0, 1920, 1080);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, menutexture);

	glUseProgram(menushaderProgram);
	glBindVertexArray(menuVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	// Swap the screen buffers

}



