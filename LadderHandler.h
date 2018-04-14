#pragma once

#include "Sprite.h"
#include <vector>
#include "VariableTexture.h"

class LadderHandler
{

public:
	void init(ShaderProgram *shaderProgram);

	void addLadder(glm::vec2 &initialPosition);
	void render();

private:

	std::vector<Sprite*> ladders;
	Texture spritesheet;
	ShaderProgram *shaderProgram;
};
