#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include "LadderHandler.h"

void LadderHandler::init(ShaderProgram *shaderProgram)
{
	spritesheet.loadFromFile("images/ladder.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	this->shaderProgram = shaderProgram;
}

void LadderHandler::addLadder(glm::vec2 &pos) {
	Sprite *sprite = Sprite::createSprite(glm::ivec2(6, 1), glm::vec2(1, 1), &spritesheet, shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 1);
	sprite->addKeyframe(0, glm::vec2(1, 1));
	sprite->changeAnimation(0);
	sprite->setPosition(pos);
	ladders.push_back(sprite);
}

void LadderHandler::render()
{
	for (Sprite* p : ladders) {
		p->render();
	}
}