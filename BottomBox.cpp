#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>
#include "BottomBox.h"

#define HABILITIES_NUMBER 7
#define TOTAL_BUTTONS 9

BottomBox::BottomBox() {}

BottomBox::~BottomBox() {}

void BottomBox::init(string *selectedHab, ShaderProgram *overlayProgram, std::function<void()> nuke, std::function<void()> pause)
{
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);

	glm::vec2 texCoords[2] = { glm::vec2(0.0f, 0.f), glm::vec2(1.0f, 1.0f) }; //creo que si pones 1 en los dos coge todo

	string habilitiesNames[] = { "control_bash.png", "control_block.png", "control_build.png", "control_climb.png", "control_dig.png", "control_float.png", "control_mine.png" };
	string habilitiesID[] = { "BASHER", "BLOCKER", "BUILDER", "CLIMBER", "DIGGER", "FLOATER", "MINER"};

	for (int i = 0; i < HABILITIES_NUMBER; ++i) {

		habilitiesTexs.push_back(new Texture());
		habilitiesTexs[i]->loadFromFile("images/buttons/" + habilitiesNames[i], TEXTURE_PIXEL_FORMAT_RGBA);
		habilitiesTexs[i]->setMinFilter(GL_NEAREST);
		habilitiesTexs[i]->setMagFilter(GL_NEAREST);

		std::function<void()> selectHabFunc = std::bind(&BottomBox::selectHab, this, habilitiesID[i]);

		glm::vec2 geom[2] = { glm::vec2(float(CAMERA_WIDTH - 1) / 2.0f - (14.0f*(float)TOTAL_BUTTONS / 2.0f) + i * 14.0f, float(CAMERA_HEIGHT - 1) - 25.0f),
			glm::vec2(float(CAMERA_WIDTH - 1) / 2.0f - (14.0f*(float)TOTAL_BUTTONS / 2.0f) + (i + 1)*14.0f, float(CAMERA_HEIGHT - 1)) };
		habilities.push_back(Button::createButton(geom, texCoords, selectHabFunc, habilitiesTexs[i], overlayProgram));

		//Text text;
		habsNums.push_back(new Text());
		for (Text* t : habsNums) {
			if (!t->init("fonts/OpenSans-Regular.ttf"))
				//if(!text.init("fonts/OpenSans-Bold.ttf"))
				//if(!text.init("fonts/DroidSerif.ttf"))
				cout << "Could not load font!!!" << endl;
		}

		habilitiesQuant.push_back(i);
	}

	habilitiesTexs.push_back(new Texture());
	habilitiesTexs[HABILITIES_NUMBER]->loadFromFile("images/buttons/control_nuke.png", TEXTURE_PIXEL_FORMAT_RGBA);
	habilitiesTexs[HABILITIES_NUMBER]->setMinFilter(GL_NEAREST);
	habilitiesTexs[HABILITIES_NUMBER]->setMagFilter(GL_NEAREST);

	std::function<void()> nukeButton = std::bind(&BottomBox::callAndSetHab, this, nuke, "NUKE");

	glm::vec2 geom[2] = { glm::vec2(float(CAMERA_WIDTH - 1) / 2.0f - (14.0f*(float)TOTAL_BUTTONS / 2.0f) + (float)HABILITIES_NUMBER * 14.0f, float(CAMERA_HEIGHT - 1) - 25.0f),
		glm::vec2(float(CAMERA_WIDTH - 1) / 2.0f - (14.0f*(float)TOTAL_BUTTONS / 2.0f) + ((float)HABILITIES_NUMBER + 1)*14.0f, float(CAMERA_HEIGHT - 1)) };
	habilities.push_back(Button::createButton(geom, texCoords, nuke, habilitiesTexs[HABILITIES_NUMBER], overlayProgram));

	habilitiesTexs.push_back(new Texture());
	habilitiesTexs[HABILITIES_NUMBER+1]->loadFromFile("images/buttons/control_pause.png", TEXTURE_PIXEL_FORMAT_RGBA);
	habilitiesTexs[HABILITIES_NUMBER+1]->setMinFilter(GL_NEAREST);
	habilitiesTexs[HABILITIES_NUMBER+1]->setMagFilter(GL_NEAREST);

	std::function<void()> pauseButton = std::bind(&BottomBox::callAndSetHab, this, pause, "PAUSE");

	geom[0] = glm::vec2(float(CAMERA_WIDTH - 1) / 2.0f - (14.0f*(float)TOTAL_BUTTONS / 2.0f) + ((float)HABILITIES_NUMBER + 1.0f) * 14.0f, float(CAMERA_HEIGHT - 1) - 25.0f);
	geom[1] = glm::vec2(float(CAMERA_WIDTH - 1) / 2.0f - (14.0f*(float)TOTAL_BUTTONS / 2.0f) + (((float)HABILITIES_NUMBER + 1.0f) + 1)*14.0f, float(CAMERA_HEIGHT - 1));
	habilities.push_back(Button::createButton(geom, texCoords, pauseButton, habilitiesTexs[HABILITIES_NUMBER+1], overlayProgram));

	position[0] = glm::vec2(float(CAMERA_WIDTH - 1) / 2.0f - (14.0f*(float)TOTAL_BUTTONS / 2.0f), float(CAMERA_HEIGHT - 1) - 25.0f);
	position[1] = glm::vec2(float(CAMERA_WIDTH - 1) / 2.0f - (14.0f*(float)TOTAL_BUTTONS / 2.0f) + (((float)TOTAL_BUTTONS -1) + 1)*14.0f, float(CAMERA_HEIGHT - 1));

	this->overlayProgram = overlayProgram;
	this->selectedHab = selectedHab;
}

void BottomBox::callAndSetHab(std::function<void()> call, string hab)
{
	call();
	*selectedHab = hab;
}

void BottomBox::selectHab(string hab)
{
	*selectedHab = hab;
}

void BottomBox::render()
{
	for (Button* b : habilities) {
		overlayProgram->use();
		overlayProgram->setUniformMatrix4f("projection", projection);
		if (b->isMouseOver()) {
			overlayProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 0.9f);
		} else if (focus) {
			overlayProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		} else {
			overlayProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 0.5f);
		}
		glm::mat4 modelview = glm::mat4(1.0f);
		overlayProgram->setUniformMatrix4f("modelview", modelview);
		b->render();
	}
	for (int i = 0; i < HABILITIES_NUMBER; ++i) {
		habsNums[i]->render(static_cast<ostringstream*>(&(ostringstream() << habilitiesQuant[i]))->str(),
			glm::vec2((float(CAMERA_WIDTH - 1) / 2.0f - (14.0f*TOTAL_BUTTONS / 2.0f) + i * 14.0f + 6.0f)*3.0f, (float(CAMERA_HEIGHT - 1) - 18.0f)*3.0f), 15, glm::vec4(0, 0, 0, 1));
	}
}

void BottomBox::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	if ((position[0].x <= mouseX / 3) && (mouseX / 3 <= position[1].x) && (position[0].y <= mouseY / 3) && (mouseY / 3 <= position[1].y))
		focus = true;
	else
		focus = false;

	for each (Button* b in habilities)
	{
		b->mouseMoved(mouseX, mouseY, bLeftButton, bRightButton);
	}
	
}