#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Menu.h"


Menu::Menu(){}

Menu::~Menu(){}

void Menu::init(std::function<void()> callback, std::function<void()> exit)
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

	playButton = new MenuButton();
	playButton->init(glm::vec2(960/2 - 100, 80), program, "images/play_button.png");
	playButton->attachCallback(callback);

	insButton = new MenuButton();
	insButton->init(glm::vec2(960 / 2 - 100, 250), program, "images/instructions_button.png");

	exitButton = new MenuButton();
	exitButton->init(glm::vec2(960 / 2 - 100, 420), program, "images/exit_button.png");
	exitButton->attachCallback(exit);

	projection = glm::ortho(0.f, float(960 - 1), float(642 - 1), 0.f);

	MusicHandler::play("songs/background.mp3", true);
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
	bg->render(texBg);
	if (playButton->isMouseOver()) {
		program.setUniform4f("color", 0.0f, 1.0f, 1.0f, 1.0f);
		//cout << "play" << endl;
	}
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

void Menu::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{ 
	std::cout << mouseX << std::endl;
	playButton->mouseMoved(mouseX, mouseY, bLeftButton, bRightButton);
	insButton->mouseMoved(mouseX, mouseY, bLeftButton, bRightButton);
	exitButton->mouseMoved(mouseX, mouseY, bLeftButton, bRightButton);
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



