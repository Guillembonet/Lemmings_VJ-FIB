#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Lemming.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum LemmingAnims
{
	WALKING_LEFT, WALKING_RIGHT, LEAVING, OUT_OF_SCENE
};


void Lemming::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	state = FALLING_RIGHT_STATE;
	spritesheet.loadFromFile("images/lemmings.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1/32.0f, 1/34.0f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);
	
		sprite->setAnimationSpeed(WALKING_RIGHT, 12);
		for(int i=0; i<8; i++)
			sprite->addKeyframe(WALKING_RIGHT, glm::vec2(float(i) / 32, 17/34.0f));
		
		sprite->setAnimationSpeed(WALKING_LEFT, 12);
		for(int i=0; i<8; i++)
			sprite->addKeyframe(WALKING_LEFT, glm::vec2(float(i) / 32, 0.0f));

		sprite->setAnimationSpeed(LEAVING, 8);
		for(int i = 0; i<8; i++)
			sprite->addKeyframe(LEAVING, glm::vec2(float(i) / 32, 13/34.0f));

		sprite->setAnimationSpeed(OUT_OF_SCENE, 1);
		sprite->addKeyframe(OUT_OF_SCENE, glm::vec2(30/32.0f, 0.0f));
		
	sprite->changeAnimation(WALKING_RIGHT);
	sprite->setPosition(initialPosition);
}

void Lemming::update(int deltaTime)
{
	int fall;

	if(sprite->update(deltaTime) == 0)
		return;

	if(state ==  FALLING_LEFT_STATE){
		fall = collisionFloor(2);
		if(fall > 0)
			sprite->position() += glm::vec2(0, fall);
		else
			state = WALKING_LEFT_STATE;
	}
	else if (state == FALLING_RIGHT_STATE) {
		fall = collisionFloor(2);
		if (fall > 0)
			sprite->position() += glm::vec2(0, fall);
		else
			state = WALKING_RIGHT_STATE;
	}
	else if (state == WALKING_LEFT_STATE) {
		sprite->position() += glm::vec2(-1, -1);
		if (collision())
		{
			sprite->position() -= glm::vec2(-1, -1);
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
		}
		else
		{
			fall = collisionFloor(3);
			if (fall > 0)
				sprite->position() += glm::vec2(0, 1);
			if (fall > 1)
				sprite->position() += glm::vec2(0, 1);
			if (fall > 2)
				state = FALLING_LEFT_STATE;
		}
	}
	else if (state == WALKING_RIGHT_STATE) {
		sprite->position() += glm::vec2(1, -1);
		if (collision())
		{
			sprite->position() -= glm::vec2(1, -1);
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
		}
		else
		{
			fall = collisionFloor(3);
			if (fall < 3)
				sprite->position() += glm::vec2(0, fall);
			else
				state = FALLING_RIGHT_STATE;
		}
	}
	else if (state == LEAVING_STATE) { 
		// Si hemos acabado de salir por la puerta, nos vamos fuera de la escena
		if (sprite->getCurrentKeyFrameIndex() == sprite->getKeyFramesLenght() - 1) {
			state = OUT_OF_SCENE_STATE;
			sprite->changeAnimation(OUT_OF_SCENE);
		}
	}

	// Si estamos caminando y nos encontramos en frente de la puerta...
	if (state == WALKING_RIGHT_STATE || state == WALKING_LEFT_STATE) {
		glm::ivec2 posBase = sprite->position();
		if (inFrontOfExitDoor(posBase.x, posBase.y)) {
			state = LEAVING_STATE;
			sprite->changeAnimation(LEAVING);
		}
	}
}

void Lemming::render()
{
	sprite->render();
}

void Lemming::setMapMask(VariableTexture *mapMask)
{
	mask = mapMask;
}

void Lemming::setExitDoorCoords(int x, int y, int w, int h) {
	exitDoor.x = x;
	exitDoor.y = y;
	exitDoor.width = w;
	exitDoor.height = h;
}

int Lemming::collisionFloor(int maxFall)
{
	bool bContact = false;
	int fall = 0;
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 16);
	while((fall < maxFall) && !bContact)
	{
		if((mask->pixel(posBase.x, posBase.y+fall) == 0) && (mask->pixel(posBase.x+1, posBase.y+fall) == 0))
			fall += 1;
		else
			bContact = true;
	}

	return fall;
}

bool Lemming::collision()
{
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 15);
	if((mask->pixel(posBase.x, posBase.y) == 0) && (mask->pixel(posBase.x+1, posBase.y) == 0))
		return false;

	return true;
}

bool Lemming::inFrontOfExitDoor(int x, int y) {
	return x > exitDoor.x && x < exitDoor.x + exitDoor.width && y < exitDoor.y && y > exitDoor.y - exitDoor.height;
}

bool Lemming::hasLeft() {
	return state == OUT_OF_SCENE_STATE;
}







