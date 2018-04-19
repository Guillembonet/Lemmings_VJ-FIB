#include "SkyDoor.h"

enum SkyDoorAnims
{
	CLOSED, MOVEMENT, OPEN
};

void SkyDoor::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, string imageUrl)
{
	spritesheet.loadFromFile(imageUrl, TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);

	sprite = Sprite::createSprite(glm::ivec2(35, 25), glm::vec2(0.1f, 1.0f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(MOVEMENT, 8);
	for (int i = 0; i<10; i++)
		sprite->addKeyframe(MOVEMENT, glm::vec2(float(i) / 10, 0.f));

	sprite->setAnimationSpeed(OPEN, 8);
	sprite->addKeyframe(OPEN, glm::vec2(float(9) / 10, 0.f));

	sprite->setAnimationSpeed(CLOSED, 8);
	sprite->addKeyframe(CLOSED, glm::vec2(float(0) / 10, 0.f));

	sprite->changeAnimation(CLOSED);
	sprite->setPosition(initialPosition);
}

void SkyDoor::update(int deltaTime) {
	currentTime += deltaTime;
	if (sprite->update(deltaTime) == 0)
		return;

	/* Primero comprobamos que el tiempo sea mayor que el delay puesto y además que las condiciones para
	que no pueda volver a entrar en estado CLOSED. 
	*/
	int delay = 600;
	if (currentTime > delay && sprite->animation() != MOVEMENT && sprite->animation() != OPEN) {
		sprite->changeAnimation(MOVEMENT);
	}

	// Cuando la animación de movimiento acaba, pasamos al estado: OPEN
	if ((sprite->animation() == MOVEMENT) && (sprite->getCurrentKeyFrameIndex() == sprite->getKeyFramesLenght() - 1)) {
		sprite->changeAnimation(OPEN);
	}
}

void SkyDoor::render()
{
	sprite->render();
}

bool SkyDoor::isDoorOpen()
{
	return sprite->animation() == OPEN;
}

