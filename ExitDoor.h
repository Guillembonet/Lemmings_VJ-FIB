#pragma once

#include "Sprite.h"
#include <iostream>

class ExitDoor
{
public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

private:
	Sprite * sprite;
	Texture spritesheet;
};