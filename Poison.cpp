#include "Poison.h"

enum PoisonAnims
{
	MOVEMENT
};

void Poison::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/poison.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);

	sprite = Sprite::createSprite(glm::ivec2(42, 20), glm::vec2(1/8.f, 1.0f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(MOVEMENT, 8);
	for (int i = 0; i<8; i++)
		sprite->addKeyframe(MOVEMENT, glm::vec2(float(i) / 8, 0.f));

	sprite->changeAnimation(MOVEMENT);
	sprite->setPosition(initialPosition);
}

void Poison::update(int deltaTime) {
	currentTime += deltaTime;
	if (sprite->update(deltaTime) == 0)
		return;
}

void Poison::render()
{
	sprite->render();
}
