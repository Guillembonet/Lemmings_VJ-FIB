#include "MenuButton.h"

enum MenuButtonAnims
{
	DEFAULT
};

void MenuButton::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, string imageUrl)
{
	spritesheet.loadFromFile(imageUrl, TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);

	sprite = Sprite::createSprite(glm::ivec2(width, height), glm::vec2(1.0f, 1.0f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(DEFAULT, 1);
	sprite->addKeyframe(DEFAULT, glm::vec2(0.f, 0.f));


	sprite->changeAnimation(DEFAULT);
	sprite->setPosition(initialPosition);
}

void MenuButton::update(int deltaTime) {
	if (sprite->update(deltaTime) == 0)
		return;
}

void MenuButton::render()
{
	sprite->render();
}

bool MenuButton::isMouseOver() {
	return mouseOver;
}

void MenuButton::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton) {
	auto pos = sprite->position();
	if (mouseX > pos.x && mouseX < pos.x + width && mouseY > pos.y && mouseY < pos.y + height) {
		if(mouseOver == false) MusicHandler::play("songs/mouseOverEffect.mp3", false);
		mouseOver = true;
		if (bLeftButton && callback != NULL) {
			std::cout << "POleeeee";
			MusicHandler::play("songs/selectEffect.mp3", false);
			MusicHandler::setVolume("songs/selectEffect.mp3", 200);
			this->callback();
		}
	}
	else mouseOver = false;
}

void MenuButton::attachCallback(std::function<void()> callback) {
	this->callback = callback;
}