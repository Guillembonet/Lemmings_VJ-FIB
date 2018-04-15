#pragma once

#include "Sprite.h"
#include <iostream>

class MousePointer
{
public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, int *leftPos, int *rightPos);
	void update(int deltaTime);
	void render();
	void MousePointer::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);
	glm::vec2 getPosition();

private:
	Sprite * sprite;
	Texture spritesheet;
	int *leftPos;
	int *rightPos;
	int mouseX, mouseY;
};