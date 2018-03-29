#pragma once

#include "Sprite.h"
#include "VariableTexture.h"
#include <iostream>

class SkyDoor
{
public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	bool isDoorOpen();

private:
	Sprite * sprite;
	Texture spritesheet;

	float currentTime;
};

