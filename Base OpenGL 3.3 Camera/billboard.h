#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cstring>
#include <string>
#include <stack>
#include "shader_s.h"
#include <vector>
//#include<glhlib/glhlib.h>
#include "camera.h"
#include "mesh.h"
#include "camera.h"
using namespace std;
using namespace glm;


float vertices[] = {
	// positions          // colors           // texture coords
	 0.5f ,  0.5f , 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // top right
	 0.5f ,  -0.5f , 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f , -0.5f , 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // bottom left
	-0.5f ,  0.5f , 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};
unsigned int indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};


class Billboard {
private:
	const float M_PI = 3.14159;

public:
	float offsetx = 0.0f;
	float offsety = 0.0f;
	float offsetz = 0.0f;
	float sensitivity = 0.5f;

	Billboard() {}

	unsigned int VAOQuads, VBOQuads, EBOQuads;

	void generateBuffers() {
		glGenVertexArrays(1, &VAOQuads);
		glGenBuffers(1, &VBOQuads);
		glGenBuffers(1, &EBOQuads);

		glBindVertexArray(VAOQuads);

		glBindBuffer(GL_ARRAY_BUFFER, VBOQuads);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOQuads);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
		// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
		glBindVertexArray(0);
	}

	void drawBillboard(vec3 center, vec3 position, vec3 size, unsigned int texture, Shader* myShader) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		myShader->setInt("myTexture1", 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		mat4 projection = perspective(radians(45.0f), 1920.0f / 1080.0f, 0.1f, 1000.0f);
		mat4 view = lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(center.x, center.y, center.z), vec3(0.0f, 1.0f, 0.0f));
		myShader->setMat4("view", view);
		myShader->setMat4("projection", projection);

		mat4 model = mat4(1.0f);
		model = translate(model, center);
		model = translate(model, position);
		model = scale(model, size);

		glBindVertexArray(VAOQuads); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		myShader->setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glDisable(GL_BLEND);

	}

	void drawBillboardInGame(vec3 center, vec3 position, vec3 size, unsigned int texture, Shader* myShader) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		myShader->setInt("myTexture1", 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		mat4 projection = perspective(radians(45.0f), 1920.0f / 1080.0f, 0.1f, 1000.0f);
		mat4 view = lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(center.x, center.y, center.z), vec3(0.0f, 1.0f, 0.0f));
		myShader->setMat4("view", view);
		myShader->setMat4("projection", projection);

		mat4 model = mat4(1.0f);
		model = translate(model, center);
		model = translate(model, position);
		model = scale(model, size);

		glBindVertexArray(VAOQuads); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		myShader->setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glDisable(GL_BLEND);

	}
	
};