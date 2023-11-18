#pragma once
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <queue>
#include "glm/glm.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "model.h"
#include "camera.h"
#include "shader_s.h"

using namespace glm;
using namespace std;

//unsigned int SCR_WIDTH = 1920;
//unsigned int SCR_HEIGHT = 1080;

class Nemico {
public:
	
	Nemico() {
		vec3 position;

		float yaw;
		float velocita;
	}

	virtual vec3 getPosition();
	virtual float getYaw();
	virtual void caduta();
	//virtual void movimentoIdle();
	//virtual void renderNemico(Model* personaggio, Shader* shader, vec3 position, Camera camera);

	vec3 position;
	float yaw;
	float velocita;
};	

vec3 Nemico::getPosition() {
	return position;
}

float Nemico::getYaw() {
	return yaw;
}

void Nemico::caduta() {

}

//void Nemico::renderNemico(Model* personaggio, Shader* shader, vec3 position, Camera camera)
//{
//	mat4 viewMatrix = camera.GetViewMatrix();
//	mat4 projMatrix = camera.GetProjMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT);
//	shader->setMat4("view", viewMatrix);
//	shader->setMat4("projection", projMatrix);
//
//	glm::mat4 modelMatrix = glm::mat4(1.0f);
//	//modelMatrix = translate(modelMatrix, car.getMidValPosition());
//	modelMatrix = translate(modelMatrix, position);
//	modelMatrix = scale(modelMatrix, vec3(0.5f, 0.5f, 0.5f));
//	modelMatrix = rotate(modelMatrix, 2.5f, vec3(0.0f, 1.0f, 0.0f));
//
//	glm::mat4 modelMatrix1 = glm::mat4(1.0f);
//
//
//	shader->setMat4("model", modelMatrix);
//
//	personaggio->Draw(shader);
//}