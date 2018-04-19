#pragma once

#include "Sprite.h"
#include "VariableTexture.h"
#include <iostream>

class Poison
{
public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, string imageUrl);
	void update(int deltaTime);
	void render();

private:
	Sprite * sprite;
	Texture spritesheet;

	float currentTime;
};

