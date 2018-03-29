#include "ExitDoor.h"

enum SkyDoorAnims
{
	OPEN
};

void ExitDoor::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/exitDoor.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);

	sprite = Sprite::createSprite(glm::ivec2(35, 25), glm::vec2(0.166f, 1.0f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(OPEN, 8);
	for (int i = 0; i<6; i++)
		sprite->addKeyframe(OPEN, glm::vec2(float(i) / 6, 0.f));


	sprite->changeAnimation(OPEN);
	sprite->setPosition(initialPosition);
}

void ExitDoor::update(int deltaTime) {
	if (sprite->update(deltaTime) == 0)
		return;
}

void ExitDoor::render()
{
	sprite->render();
}

