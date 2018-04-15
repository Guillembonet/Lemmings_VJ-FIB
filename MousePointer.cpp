#include "MousePointer.h"

enum MousePointerAnims
{
	DEFAULT
};

void MousePointer::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, int *leftPos, int *rightPos)
{
	spritesheet.loadFromFile("images/cursor2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);

	sprite = Sprite::createSprite(glm::ivec2(15, 15), glm::vec2(1, 1.0f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(DEFAULT, 8);
	sprite->addKeyframe(DEFAULT, glm::vec2(0, 0.f));


	sprite->changeAnimation(DEFAULT);
	sprite->setPosition(initialPosition);

	this->leftPos = leftPos;
	this->rightPos = rightPos;
}

void MousePointer::update(int deltaTime) {
	if (sprite->update(deltaTime) == 0)
		return;
	sprite->setPosition(glm::vec2(mouseX / 3 + (*leftPos) - 7, mouseY / 3 - 7));
}

void MousePointer::render()
{
	sprite->render();
}

void MousePointer::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	sprite->setPosition(glm::vec2(mouseX / 3 + (*leftPos) - 7, mouseY / 3 - 7));
	this->mouseX = mouseX;
	this->mouseY = mouseY;
}

glm::vec2 MousePointer::getPosition() {
	return sprite->position();
}