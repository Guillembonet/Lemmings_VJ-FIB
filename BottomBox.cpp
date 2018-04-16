#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>
#include "BottomBox.h"

#define HABILITIES_NUMBER 7
#define TOTAL_BUTTONS 13

static float buttonsWidth = 17.0f;
static float buttonsHeight = 30.0f;

BottomBox::BottomBox() {}

BottomBox::~BottomBox() {}

void BottomBox::init(int *selectedHab, ShaderProgram *overlayProgram, std::function<void()> nuke, std::function<void()> pause, std::function<void()> faster, std::function<void()> slower, std::function<void()> fasterGen, std::function<void()> slowerGen)
{
	currentTime = 0.0f;

	projection = glm::ortho(0.f, float(WIDTH - 1), float(HEIGHT - 1), 0.f);

	glm::vec2 texCoords[2] = { glm::vec2(0.0f, 0.f), glm::vec2(1.0f, 1.0f) };

	string habilitiesNames[] = { "control_bash.png", "control_block.png", "control_build.png", "control_climb.png", "control_dig.png", "control_float.png", "control_mine.png" };

	for (int i = 0; i < HABILITIES_NUMBER; ++i) {

		habilitiesTexs.push_back(new Texture());
		habilitiesTexs[i]->loadFromFile("images/buttons/" + habilitiesNames[i], TEXTURE_PIXEL_FORMAT_RGBA);
		habilitiesTexs[i]->setMinFilter(GL_NEAREST);
		habilitiesTexs[i]->setMagFilter(GL_NEAREST);

		glm::vec2 geom[2] = {glm::vec2(i * buttonsWidth, float(HEIGHT - 1) - buttonsHeight),
			glm::vec2((i + 1)*buttonsWidth, float(HEIGHT - 1)) };
		habilities.push_back(Button::createButton(geom, texCoords, std::bind(&BottomBox::selectHab, this, i), habilitiesTexs[i], overlayProgram));

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

	//PAUSE
	habilitiesTexs.push_back(new Texture());
	habilitiesTexs[HABILITIES_NUMBER]->loadFromFile("images/buttons/control_pause.png", TEXTURE_PIXEL_FORMAT_RGBA);
	habilitiesTexs[HABILITIES_NUMBER]->setMinFilter(GL_NEAREST);
	habilitiesTexs[HABILITIES_NUMBER]->setMagFilter(GL_NEAREST);

	glm::vec2 geom[2] = { glm::vec2((float)HABILITIES_NUMBER * buttonsWidth, float(HEIGHT - 1) - buttonsHeight),
		glm::vec2(((float)HABILITIES_NUMBER + 1)*buttonsWidth, float(HEIGHT - 1)) };
	habilities.push_back(Button::createButton(geom, texCoords, std::bind(&BottomBox::callAndSetHab, this, pause, 7), habilitiesTexs[HABILITIES_NUMBER], overlayProgram));

	//NUKE
	habilitiesTexs.push_back(new Texture());
	habilitiesTexs[HABILITIES_NUMBER + 1]->loadFromFile("images/buttons/control_nuke.png", TEXTURE_PIXEL_FORMAT_RGBA);
	habilitiesTexs[HABILITIES_NUMBER + 1]->setMinFilter(GL_NEAREST);
	habilitiesTexs[HABILITIES_NUMBER + 1]->setMagFilter(GL_NEAREST);

	geom[0] = glm::vec2(((float)HABILITIES_NUMBER + 1.0f) * buttonsWidth, float(HEIGHT - 1) - buttonsHeight);
	geom[1] = glm::vec2(((float)HABILITIES_NUMBER + 2.0f) * buttonsWidth, float(HEIGHT - 1));
	habilities.push_back(Button::createButton(geom, texCoords, std::bind(&BottomBox::callAndSetHab, this, nuke, 8), habilitiesTexs[HABILITIES_NUMBER + 1], overlayProgram));

	//ADD SPEED
	habilitiesTexs.push_back(new Texture());
	habilitiesTexs[HABILITIES_NUMBER + 2]->loadFromFile("images/buttons/control_addspeed.png", TEXTURE_PIXEL_FORMAT_RGBA);
	habilitiesTexs[HABILITIES_NUMBER + 2]->setMinFilter(GL_NEAREST);
	habilitiesTexs[HABILITIES_NUMBER + 2]->setMagFilter(GL_NEAREST);

	geom[0] = glm::vec2(((float)HABILITIES_NUMBER + 2.0f) * buttonsWidth, float(HEIGHT - 1) - buttonsHeight);
	geom[1] = glm::vec2(((float)HABILITIES_NUMBER + 3.0f) * buttonsWidth, float(HEIGHT - 1));
	habilities.push_back(Button::createButton(geom, texCoords, std::bind(&BottomBox::callAndSetHab, this, faster, 8), habilitiesTexs[HABILITIES_NUMBER + 2], overlayProgram));

	//DECREASE SPEED
	habilitiesTexs.push_back(new Texture());
	habilitiesTexs[HABILITIES_NUMBER + 3]->loadFromFile("images/buttons/control_decreasespeed.png", TEXTURE_PIXEL_FORMAT_RGBA);
	habilitiesTexs[HABILITIES_NUMBER + 3]->setMinFilter(GL_NEAREST);
	habilitiesTexs[HABILITIES_NUMBER + 3]->setMagFilter(GL_NEAREST);

	geom[0] = glm::vec2(((float)HABILITIES_NUMBER + 3.0f) * buttonsWidth, float(HEIGHT - 1) - buttonsHeight);
	geom[1] = glm::vec2(((float)HABILITIES_NUMBER + 4.0f) * buttonsWidth, float(HEIGHT - 1));
	habilities.push_back(Button::createButton(geom, texCoords, std::bind(&BottomBox::callAndSetHab, this, slower, 8), habilitiesTexs[HABILITIES_NUMBER + 3], overlayProgram));

	//MORE GENERATION SPEED
	habilitiesTexs.push_back(new Texture());
	habilitiesTexs[HABILITIES_NUMBER + 4]->loadFromFile("images/buttons/control_more.png", TEXTURE_PIXEL_FORMAT_RGBA);
	habilitiesTexs[HABILITIES_NUMBER + 4]->setMinFilter(GL_NEAREST);
	habilitiesTexs[HABILITIES_NUMBER + 4]->setMagFilter(GL_NEAREST);

	geom[0] = glm::vec2(((float)HABILITIES_NUMBER + 4.0f) * buttonsWidth, float(HEIGHT - 1) - buttonsHeight);
	geom[1] = glm::vec2(((float)HABILITIES_NUMBER + 5.0f) * buttonsWidth, float(HEIGHT - 1));
	habilities.push_back(Button::createButton(geom, texCoords, std::bind(&BottomBox::callAndSetHab, this, fasterGen, 8), habilitiesTexs[HABILITIES_NUMBER + 4], overlayProgram));

	//LESS GENERATION SPEED
	habilitiesTexs.push_back(new Texture());
	habilitiesTexs[HABILITIES_NUMBER + 5]->loadFromFile("images/buttons/control_less.png", TEXTURE_PIXEL_FORMAT_RGBA);
	habilitiesTexs[HABILITIES_NUMBER + 5]->setMinFilter(GL_NEAREST);
	habilitiesTexs[HABILITIES_NUMBER + 5]->setMagFilter(GL_NEAREST);

	geom[0] = glm::vec2(((float)HABILITIES_NUMBER + 5.0f) * buttonsWidth, float(HEIGHT - 1) - buttonsHeight);
	geom[1] = glm::vec2(((float)HABILITIES_NUMBER + 6.0f) * buttonsWidth, float(HEIGHT - 1));
	habilities.push_back(Button::createButton(geom, texCoords, std::bind(&BottomBox::callAndSetHab, this, slowerGen, 8), habilitiesTexs[HABILITIES_NUMBER + 5], overlayProgram));

	//BOX POSITION
	position[0] = glm::vec2(0, float(HEIGHT - 1) - buttonsHeight);
	position[1] = glm::vec2((float)TOTAL_BUTTONS * buttonsWidth, float(HEIGHT - 1));

	if (!time.init("fonts/OpenSans-Regular.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;

	if (!in.init("fonts/OpenSans-Regular.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;

	if (!out.init("fonts/OpenSans-Regular.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;

	this->overlayProgram = overlayProgram;
	this->selectedHab = selectedHab;
}

void BottomBox::callAndSetHab(std::function<void()> call, int hab)
{
	*selectedHab = hab;
	call();
}

void BottomBox::selectHab(int hab)
{
	*selectedHab = hab;
}

void BottomBox::update(int deltaTime) {
	currentTime += deltaTime;
}

void BottomBox::render()
{
	int i = 0;
	for (Button* b : habilities) {
		overlayProgram->use();
		overlayProgram->setUniformMatrix4f("projection", projection);
		if (*selectedHab == i && i < 8) {
			overlayProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 0.8f);
		}
		else if (b->isMouseOver()) {
			overlayProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 0.9f);
		} else {
			overlayProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		}
		glm::mat4 modelview = glm::mat4(1.0f);
		overlayProgram->setUniformMatrix4f("modelview", modelview);
		b->render();
		++i;
	}
	for (int i = 0; i < HABILITIES_NUMBER; ++i) {
		habsNums[i]->render(static_cast<ostringstream*>(&(ostringstream() << habilitiesQuant[i]))->str(),
			glm::vec2((i * buttonsWidth + buttonsWidth/2.0f - 1.0f)*3.0f, (float(HEIGHT) - 2.0f)*3.0f), 15, glm::vec4(0, 0, 0, 1));
	}

	time.render("Time left: " + static_cast<ostringstream*>(&(ostringstream() << currentTime))->str(),
		glm::vec2((TOTAL_BUTTONS * buttonsWidth + buttonsWidth / 2.0f - 1.0f)*3.0f, (float(HEIGHT) - 2.0f)*3.0f), 20, glm::vec4(1, 1, 1, 1));

	in.render("In: " + static_cast<ostringstream*>(&(ostringstream() << 23))->str() + "%",
		glm::vec2((TOTAL_BUTTONS * buttonsWidth + buttonsWidth / 2.0f - 1.0f)*3.0f, (float(HEIGHT) + 8.0f)*3.0f), 20, glm::vec4(1, 1, 1, 1));

	out.render("Out: " + static_cast<ostringstream*>(&(ostringstream() << 10))->str(),
		glm::vec2((TOTAL_BUTTONS * buttonsWidth + buttonsWidth / 2.0f - 1.0f)*3.0f, (float(HEIGHT) + 18.0f)*3.0f), 20, glm::vec4(1, 1, 1, 1));
}

void BottomBox::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	for each (Button* b in habilities)
	{
		b->mouseMoved(mouseX, mouseY, bLeftButton, bRightButton);
	}
	
}