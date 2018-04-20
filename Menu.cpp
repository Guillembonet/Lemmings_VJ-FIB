#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Menu.h"


Menu::Menu(){}

Menu::~Menu(){}

void Menu::setState(state newState){
	currentState = newState;
}

void Menu::init(vector<std::function<void()>> callbacks, std::function<void()> exit)
{

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	currentTime = 0.0f;
	
	glm::vec2 texCoords[2] = { glm::vec2(0.0f, 0.f), glm::vec2(1.0f, 1.0f) };

	initShaders();

	glm::vec2 geomBg[2] = { glm::vec2(0,0), glm::vec2(float(960), float(642)) };

	texBg.loadFromFile("images/menubg.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texBg.setMinFilter(GL_NEAREST);
	texBg.setMagFilter(GL_NEAREST);
	bg = TexturedQuad::createTexturedQuad(geomBg, texCoords, program);

	texInsBg.loadFromFile("images/instructions.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texInsBg.setMinFilter(GL_NEAREST);
	texInsBg.setMagFilter(GL_NEAREST);

	playButton = new MenuButton();
	playButton->init(glm::vec2(960/2 - 100, 80), program, "images/play_button.png");
	playButton->attachCallback(std::bind(&Menu::setState, this, LEVELS));

	insButton = new MenuButton();
	insButton->init(glm::vec2(960 / 2 - 100, 250), program, "images/instructions_button.png");
	insButton->attachCallback(std::bind(&Menu::setState, this, INSTRUCTIONS));

	exitButton = new MenuButton();
	exitButton->init(glm::vec2(960 / 2 - 100, 420), program, "images/exit_button.png");
	exitButton->attachCallback(exit);

	level1 = new MenuButton();
	level1->init(glm::vec2(960 / 2 - 100, 80), program, "images/easy_button.png");
	level1->attachCallback(callbacks[0]);

	level2 = new MenuButton();
	level2->init(glm::vec2(960 / 2 - 100, 250), program, "images/medium_button.png");
	level2->attachCallback(callbacks[1]);

	level3 = new MenuButton();
	level3->init(glm::vec2(960 / 2 - 100, 420), program, "images/hard_button.png");
	level3->attachCallback(callbacks[2]);

	back = new MenuButton();
	back->init(glm::vec2(960*4 / 5 - 100, 480), program, "images/back_button.png");
	back->attachCallback(std::bind(&Menu::setState, this, BASIC));

	projection = glm::ortho(0.f, float(960 - 1), float(642 - 1), 0.f);

	MusicHandler::play("songs/background.mp3", true);
	MusicHandler::setVolume("songs/background.mp3", 120);
}

void Menu::update(int deltaTime)
{
	currentTime += deltaTime;
}

void Menu::render()
{
	glm::mat4 modelview;

	program.use();
	program.setUniformMatrix4f("projection", projection);
	program.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	program.setUniformMatrix4f("modelview", modelview);
	if (currentState == INSTRUCTIONS)
		bg->render(texInsBg);
	else
		bg->render(texBg);
	if (currentState == BASIC) {
		if (playButton->isMouseOver())
			program.setUniform4f("color", 0.0f, 1.0f, 1.0f, 1.0f);
		else
			program.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		playButton->render();
		if (insButton->isMouseOver())
			program.setUniform4f("color", 0.0f, 1.0f, 1.0f, 1.0f);
		else
			program.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		insButton->render();
		if (exitButton->isMouseOver())
			program.setUniform4f("color", 0.5f, 1.0f, 0.5f, 1.0f);
		else
			program.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		exitButton->render();
	}
	else if (currentState == LEVELS) {
		if (level1->isMouseOver())
			program.setUniform4f("color", 0.0f, 1.0f, 1.0f, 1.0f);
		else
			program.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		level1->render();
		if (level2->isMouseOver())
			program.setUniform4f("color", 0.0f, 1.0f, 1.0f, 1.0f);
		else
			program.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		level2->render();
		if (level3->isMouseOver())
			program.setUniform4f("color", 0.5f, 1.0f, 0.5f, 1.0f);
		else
			program.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		level3->render();
		if (back->isMouseOver())
			program.setUniform4f("color", 0.5f, 1.0f, 0.5f, 1.0f);
		else
			program.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		back->render();
	}
	else if (currentState == INSTRUCTIONS) {
		if (back->isMouseOver())
			program.setUniform4f("color", 0.5f, 1.0f, 0.5f, 1.0f);
		else
			program.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		back->render();
	}
}

void Menu::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{ 
	if (currentState == BASIC) {
		playButton->mouseMoved(mouseX, mouseY, bLeftButton, bRightButton);
		insButton->mouseMoved(mouseX, mouseY, bLeftButton, bRightButton);
		exitButton->mouseMoved(mouseX, mouseY, bLeftButton, bRightButton);
	}
	else if (currentState == LEVELS) {
		level1->mouseMoved(mouseX, mouseY, bLeftButton, bRightButton);
		level2->mouseMoved(mouseX, mouseY, bLeftButton, bRightButton);
		level3->mouseMoved(mouseX, mouseY, bLeftButton, bRightButton);
		back->mouseMoved(mouseX, mouseY, bLeftButton, bRightButton);
	}
	else if (currentState == INSTRUCTIONS) {
		back->mouseMoved(mouseX, mouseY, bLeftButton, bRightButton);
	}
}

void Menu::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/maskedTexture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/maskedTexture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	program.init();
	program.addShader(vShader);
	program.addShader(fShader);
	program.link();
	if (!program.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << program.log() << endl << endl;
	}
	program.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



