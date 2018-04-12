#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include "Explosion.h"

#define PI 3.14159265

void Explosion::init(ShaderProgram *shaderProgram)
{
	spritesheet.loadFromFile("images/particle.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	for (int i = 0; i < 5; ++i) {
		particles.push_back(Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1, 1), &spritesheet, shaderProgram));
		particles[i]->setNumberAnimations(1);
		particles[i]->setAnimationSpeed(0, 1);
		particles[i]->addKeyframe(0, glm::vec2(1,1));
		particles[i]->changeAnimation(0);
		speeds.push_back((rand() % 30 + 1) / 10.0f + 5.0f);
	}
}

void Explosion::start(const glm::vec2 &initialPosition)
{
	pos0 = initialPosition;
	for (Sprite* p : particles) {
		p->setPosition(initialPosition);
	}
	explosionStartTime = 0.0f;
}

void Explosion::update(int deltaTime)
{
	explosionStartTime += deltaTime;
	float a = 0;
	int i = 0;
	for (Sprite* p : particles) {
		float t = explosionStartTime / 1000.0f;
		float x = speeds[i] * cos(a * PI / 180.0f) * t;
		float y = -speeds[i] * sin(a * PI / 180.0f) * t + 4.0f*t*t;
		p->position() += glm::vec2(x,y);
		a += 36;
		++i;
	}
}

void Explosion::render()
{
	for (Sprite* p : particles) {
		p->render();
	}
}