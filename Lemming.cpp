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
	WALKING_LEFT, WALKING_RIGHT, LEAVING, OUT_OF_SCENE, FALLING_LEFT, FALLING_RIGHT, DIGGING
};


void Lemming::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	state = FALLING_RIGHT_STATE;
	spritesheet.loadFromFile("images/lemmings.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1/32.0f, 1/34.0f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(7);
	
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

		sprite->setAnimationSpeed(FALLING_LEFT, 8);
		for (int i = 0; i<4; i++)
			sprite->addKeyframe(FALLING_LEFT, glm::vec2(float(i) / 32, 1 / 34.0f));

		sprite->setAnimationSpeed(FALLING_RIGHT, 8);
		for (int i = 0; i<4; i++)
			sprite->addKeyframe(FALLING_RIGHT, glm::vec2(float(i) / 32, 18 / 34.0f));

		sprite->setAnimationSpeed(DIGGING, 10);
		for (int i = 0; i<16; i++)
			sprite->addKeyframe(DIGGING, glm::vec2(float(i) / 32, 11 / 34.0f));
		

	sprite->changeAnimation(FALLING_RIGHT);
	side = RIGHT;
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
		else {
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
		}
	}
	else if (state == FALLING_RIGHT_STATE) {
		fall = collisionFloor(2);
		if (fall > 0)
			sprite->position() += glm::vec2(0, fall);
		else {
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
		}
	}
	else if (state == WALKING_LEFT_STATE) {
		sprite->position() += glm::vec2(-1, -1);
		if (collision())
		{
			sprite->position() -= glm::vec2(-1, -1);
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
			side = RIGHT;
		}
		else
		{
			fall = collisionFloor(3);
			if (fall > 0)
				sprite->position() += glm::vec2(0, 1);
			if (fall > 1)
				sprite->position() += glm::vec2(0, 1);
			if (fall > 2) {
				sprite->changeAnimation(FALLING_LEFT);
				state = FALLING_LEFT_STATE;
			}
				
		}
	}
	else if (state == WALKING_RIGHT_STATE) {
		sprite->position() += glm::vec2(1, -1);
		if (collision())
		{
			sprite->position() -= glm::vec2(1, -1);
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
			side = LEFT;
		}
		else
		{
			fall = collisionFloor(3);
			if (fall < 3)
				sprite->position() += glm::vec2(0, fall);
			else {
				sprite->changeAnimation(FALLING_RIGHT);
				state = FALLING_RIGHT_STATE;
			}	
		}
	}
	else if (state == LEAVING_STATE) { 
		// Si hemos acabado de salir por la puerta, nos vamos fuera de la escena
		if (sprite->getCurrentKeyFrameIndex() == sprite->getKeyFramesLenght() - 1) {
			state = OUT_OF_SCENE_STATE;
			sprite->changeAnimation(OUT_OF_SCENE);
		}
	}
	else if (state == DIGGING_STATE) {
		sprite->position() += glm::vec2(0, 0.2);
		glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
		posBase += glm::ivec2(7, 16);

		fall = collisionFloorWithCoords(4, posBase.x, posBase.y-2.2);
		//std::cout << fall << std::endl;
		if (fall < 4) {
			for (int i = -5; i <= 5; i++)
			{
				mask->setPixel(posBase.x + i, posBase.y - 2.2, 0);
			}
		}
		else {
			if (side == RIGHT) {
				state = FALLING_RIGHT_STATE;
				sprite->changeAnimation(FALLING_RIGHT);
			}
			else {
				state = FALLING_LEFT_STATE;
				sprite->changeAnimation(FALLING_LEFT);
			}
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
	std::cout << "otra vez" << std::endl;
}

void Lemming::setExitDoorCoords(int x, int y, int w, int h) {
	exitDoor.x = x;
	exitDoor.y = y;
	exitDoor.width = w;
	exitDoor.height = h;
}

int Lemming::collisionFloor(int maxFall)
{
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	posBase += glm::ivec2(7, 16);
	return collisionFloorWithCoords(maxFall, posBase.x, posBase.y);
}

int Lemming::collisionFloorWithCoords(int maxFall, int x, int y) {
	bool bContact = false;
	int fall = 0;

	while ((fall < maxFall) && !bContact)
	{
		if ((mask->pixel(x, y + fall) == 0) && (mask->pixel(x + 1, y + fall) == 0))
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
	
	std::cout << "Colision" << std::endl;
	return true;
}

bool Lemming::inFrontOfExitDoor(int x, int y) {
	return x > exitDoor.x && x < exitDoor.x + exitDoor.width && y < exitDoor.y && y > exitDoor.y - exitDoor.height;
}

bool Lemming::hasLeft() {
	return state == OUT_OF_SCENE_STATE;
}

void Lemming::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton) {
	auto pos = sprite->position();
	if (isLemmingSelected(pos.x, pos.y, mouseX, mouseY) && bLeftButton &&(state == WALKING_LEFT_STATE || state == WALKING_RIGHT_STATE)) {
		state = DIGGING_STATE;
		sprite->changeAnimation(DIGGING);
		sprite->position() += glm::vec2(0, 0.5);
	}
}

/* x, y are lemming coords and i, j are mouse coords*/
bool Lemming::isLemmingSelected(int x, int y, int i, int j) {
	// We normalize mouse coords
	i = i / 3 - 7;
	j = j / 3 - 7;

	int constN = 6;

	int xx = x - constN;
	int	width = constN * 2;
	int yy = y - constN;
	int height = constN * 2;
	return i > xx && i < xx + width && j > yy && j < yy + height;
}






