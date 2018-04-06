#pragma once

#include "Sprite.h"
#include <iostream>

class MousePointer
{
public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void MousePointer::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

private:
	Sprite * sprite;
	Texture spritesheet;
};