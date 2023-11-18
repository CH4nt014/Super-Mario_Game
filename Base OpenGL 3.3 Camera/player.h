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
#include "shader_s.h"

using namespace glm;
using namespace std;

enum Direction {
	PG_FORWARD,
	PG_BACKWARD,
	PG_LEFT,
	PG_RIGHT,
	PG_UP,
	PG_DOWN,
	PG_JUMP
};

const int jumpForce = 3;
const int gravity = -1;

class Player {
public:

	Player(){
		vec3 position;
		vec3 front;
		vec3 up;
		float yaw;

		float speed;
		float angularVelocity;
		float acceleration;
		float staticAcceleration;
		float jump;
		float maxJump;
		float rotation;
		bool isJumping;

		queue<float> historyYaw;
		int delayFrameNum = 12;
		float delayYaw;

		queue<vec3> historyPosition;
		vec3 delayPosition;

		
	}

	Player(vec3 pos): yaw(0.0f), delayYaw(0.0f), speed(0.0f), angularVelocity(80.0f), acceleration(10.0f), staticAcceleration(13.0f), jump(0.0f), maxJump(1.5f), isJumping(false) {
		position = pos;
		while (historyPosition.size())
			historyPosition.pop();
		while (historyYaw.size())
			historyYaw.pop();
		updateFront();
	}

	virtual vec3 getPosition();
	virtual float getYaw();
	virtual float getDelayYaw();
	virtual float getYawDif();
	virtual float getMidValYaw();
	virtual vec3 getMidValPosition();
	virtual mat4 getViewMatrix(vec3 cameraPosition);
	virtual void ProcessKeyboard(Direction direction, float deltaTime);
	virtual void ProcessKeyboard_static(float detaTime);
	virtual void UpdateDelayYaw();
	virtual void UpdateDelayPosition();
	virtual void setYaw(float delay);

	vec3 position;
	vec3 front;
	vec3 up;
	float yaw;

	float speed;
	float angularVelocity;
	float acceleration;
	float staticAcceleration;
	float jump;
	float maxJump;
	float rotation;
	bool isJumping;


	queue<float> historyYaw;
	int delayFrameNum = 2;
	float delayYaw;

	queue<vec3> historyPosition;
	vec3 delayPosition;

private:

	void updateFront()
	{
		vec3 Front;
		Front.x = -sin(radians(getYaw()));
		Front.y = 0.0f;
		Front.z = -cos(radians(getYaw()));
		front = normalize(Front);
	}
};

vec3 Player::getPosition(){
    return position;
}

float Player::getYaw(){
    return yaw;
}

float Player::getDelayYaw(){
    return delayYaw;
}

void Player::setYaw(float delay) {
	yaw = delay;
	//yaw = yaw;
}

float Player::getYawDif(){
    return yaw - delayYaw;
}

float Player::getMidValYaw()
{
    return (delayYaw + yaw) / 2;
}

vec3 Player::getMidValPosition()
{
    /*return (delayPosition + position)/2.0f;*/
	return position + vec3(0.0, -2.0, 0.0);
}

mat4 Player::getViewMatrix(vec3 cameraPosition)
{
    return lookAt(cameraPosition, position, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Player::ProcessKeyboard(Direction direction, float deltaTime)
{
	if (direction == PG_JUMP) {
		if (!isJumping) {
			isJumping = true;
			speed = jumpForce;
		}
		if (isJumping) {
			// Applica la forza di gravità per simulare il movimento verso il basso.
			speed += gravity;

			// Aggiorna la posizione in base alla velocità.
			position += speed;

			// Controlla se il giocatore ha toccato il suolo.
			if (position.y <= 0 ){
				position.y = 0;
				isJumping = false;
				speed = 0;
			}
		}
	}
	if (direction == PG_FORWARD) position += front * 0.3f;
	if (direction == PG_BACKWARD) position -= front * 0.3f;
	if (direction == PG_LEFT)yaw += angularVelocity * deltaTime;
	if (direction == PG_RIGHT)yaw -= angularVelocity * deltaTime;
	if (direction == PG_UP) position.y += 0.5;
	if (direction == PG_DOWN)position.y -= 0.5;
	updateFront();
}

void Player::UpdateDelayYaw()
{;
    historyYaw.push(yaw);
    while (historyYaw.size() > delayFrameNum) {
        historyYaw.pop();
    }
    delayYaw = historyYaw.front();
}

void Player::UpdateDelayPosition()
{
    historyPosition.push(position);
    while (historyPosition.size() > delayFrameNum) {
        historyPosition.pop();
    }
    delayPosition = historyPosition.front();
}

void Player::ProcessKeyboard_static(float deltaTime) {
	if (speed > 0 && speed - staticAcceleration * deltaTime > 0) {
		speed -= staticAcceleration * deltaTime;
		//speed -= 0;
		position += front * speed * deltaTime;
	}
	else if (speed < 0 && speed + staticAcceleration * deltaTime < 0) {
		//speed += 0;
		speed += staticAcceleration * deltaTime;
		position += front * speed * deltaTime;
	}
	else if (speed < 0 && speed + staticAcceleration * deltaTime > 0) {
		speed = 0;
		position += front * speed * deltaTime;
	}
	else if (speed > 0 && speed - staticAcceleration * deltaTime < 0) {
		speed = 0;
		position -= front * speed * deltaTime;
	}
	updateFront();
}