#pragma once

#include "Sprite.h"
#include <iostream>

class MenuButton
{
public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, string imageUrl);
	void update(int deltaTime);
	void render();

	bool isMouseOver();
	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

private:
	Sprite * sprite;
	Texture spritesheet;
	bool mouseOver;
	int width = 130;
	int height = 40;
};