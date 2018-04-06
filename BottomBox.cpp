#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include "BottomBox.h"

#define HABILITIES_NUMBER 4

BottomBox::BottomBox() {}

BottomBox::~BottomBox() {}

void BottomBox::init(std::function<void()> callback, ShaderProgram *overlayProgram)
{
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);

	glm::vec2 texCoords[2] = { glm::vec2(0.0f, 0.f), glm::vec2(1.0f, 1.0f) }; //creo que si pones 1 en los dos coge todo

	string habilitiesNames[] = { "control_bash.png", "control_block.png", "control_build.png", "control_climb.png" };

	for (int i = 0; i < HABILITIES_NUMBER; ++i) {
		habilitiesTexs.push_back(new Texture());
		habilitiesTexs[i]->loadFromFile("images/buttons/" + habilitiesNames[i], TEXTURE_PIXEL_FORMAT_RGBA);
		habilitiesTexs[i]->setMinFilter(GL_NEAREST);
		habilitiesTexs[i]->setMagFilter(GL_NEAREST);
	}

	for (int i = 0; i < HABILITIES_NUMBER; ++i) {
		glm::vec2 geom[2] = { glm::vec2(float(CAMERA_WIDTH - 1) / 2.0f - (20.0f*HABILITIES_NUMBER / 2.0f) + i * 20.0f, float(CAMERA_HEIGHT - 1) - 25.0f),
			glm::vec2(float(CAMERA_WIDTH - 1) / 2.0f - (20.0f*HABILITIES_NUMBER / 2.0f) + (i + 1)*20.0f, float(CAMERA_HEIGHT - 1)) };
		habilities.push_back(Button::createButton(geom, texCoords, callback, habilitiesTexs[i], overlayProgram));
	}

	this->overlayProgram = overlayProgram;

}

void BottomBox::render()
{
	overlayProgram->use();
	overlayProgram->setUniformMatrix4f("projection", projection);
	overlayProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	glm::mat4 modelview = glm::mat4(1.0f);
	overlayProgram->setUniformMatrix4f("modelview", modelview);
	for (Button* b : habilities) {
		b->render();
	}
}