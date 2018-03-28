#include "SkyDoor.h"

void SkyDoor::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/skyDoor.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);

	sprite = Sprite::createSprite(glm::ivec2(35, 25), glm::vec2(0.1f, 1.0f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(0, 8);
	for (int i = 0; i<10; i++)
		sprite->addKeyframe(0, glm::vec2(float(i) / 10, 0.f));

	sprite->setAnimationSpeed(1, 8);
	sprite->addKeyframe(1, glm::vec2(float(9) / 10, 0.f));

	sprite->changeAnimation(0);
	sprite->setPosition(initialPosition);
}

void SkyDoor::update(int deltaTime) {
	if (sprite->update(deltaTime) == 0)
		return;

	if (sprite->getCurrentKeyFrameIndex() == sprite->getKeyFramesLenght() - 1) {
		sprite->changeAnimation(1);
	}
}

void SkyDoor::render()
{
	sprite->render();
}

bool SkyDoor::isDoorOpen()
{
	return sprite->animation() == 1;
}

