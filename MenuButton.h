#pragma once

#include "Sprite.h"
#include <iostream>
#include <functional>

class MenuButton
{
public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, string imageUrl);
	void update(int deltaTime);
	void render();

	bool isMouseOver();
	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);
	void attachCallback(std::function<void()> callback);

private:
	Sprite * sprite;
	Texture spritesheet;
	bool mouseOver;
	int width = 200;
	int height = 100;

	std::function<void()> callback;
};