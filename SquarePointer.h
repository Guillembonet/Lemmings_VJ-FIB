#pragma once

#include "Sprite.h"
#include "VariableTexture.h"
#include <iostream>

class SquarePointer
{
public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void display(glm::vec2 &newPos);
	void hidde();

private:
	Sprite * sprite;
	Texture spritesheet;

	enum squareState {
		HIDDEN_STATE, DEFAULT_STATE
	};

	float currentTime;
	squareState state;
};