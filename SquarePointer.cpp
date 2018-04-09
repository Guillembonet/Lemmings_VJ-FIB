#include "SquarePointer.h"

enum SquarePointerAnims
{
	DEFAULT
};

void SquarePointer::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/squarePointer.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);

	sprite = Sprite::createSprite(glm::ivec2(12, 12), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(DEFAULT, 8);
	sprite->addKeyframe(DEFAULT, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(DEFAULT);
	sprite->setPosition(initialPosition);

	state = HIDDEN_STATE;
}

void SquarePointer::update(int deltaTime) {
	currentTime += deltaTime;
	if (sprite->update(deltaTime) == 0)
		return;
}

void SquarePointer::render()
{
	if (state == DEFAULT_STATE) sprite->render();
}

void SquarePointer::display(glm::vec2 & newPos) {
	state = DEFAULT_STATE;
	sprite->setPosition(newPos);
}

void SquarePointer::hidde() {
	state = HIDDEN_STATE;
}