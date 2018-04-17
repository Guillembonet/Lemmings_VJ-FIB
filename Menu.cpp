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

	glm::vec2 geomBg[2] = { glm::vec2(0,0), glm::vec2(float(WIDTH), float(HEIGHT)) };

	texBg.loadFromFile("images/menubg.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texBg.setMinFilter(GL_NEAREST);
	texBg.setMagFilter(GL_NEAREST);
	bg = TexturedQuad::createTexturedQuad(geomBg, texCoords, program);

	glm::vec2 geomPlay[2] = { glm::vec2(float(WIDTH) / 3.0f, float(HEIGHT) / 10.0f), glm::vec2(float(WIDTH) * 2.0f / 3.0f, float(HEIGHT)*3.0f / 10.0f) };

	texPlay.loadFromFile("images/play_button.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texPlay.setMinFilter(GL_NEAREST);
	texPlay.setMagFilter(GL_NEAREST);
	playButton = Button::createButton(geomPlay, texCoords, callback, &texPlay, &program);

	glm::vec2 geomIns[2] = { glm::vec2(float(WIDTH) / 3.0f, float(HEIGHT)*4.0 / 10.0f), glm::vec2(float(WIDTH) * 2.0f / 3.0f, float(HEIGHT)*6.0f / 10.0f) };

	std::function<void()> none;

	texIns.loadFromFile("images/instructions_button.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texIns.setMinFilter(GL_NEAREST);
	texIns.setMagFilter(GL_NEAREST);
	
	insButton = Button::createButton(geomIns, texCoords, none, &texIns, &program);

	glm::vec2 geomExit[2] = { glm::vec2(float(WIDTH) / 3.0f, float(HEIGHT)*7.0 / 10.0f), glm::vec2(float(WIDTH) * 2.0f / 3.0f, float(HEIGHT)*9.0f / 10.0f) };

	texExit.loadFromFile("images/exit_button.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texExit.setMinFilter(GL_NEAREST);
	texExit.setMagFilter(GL_NEAREST);
	exitButton = Button::createButton(geomExit, texCoords, exit, &texExit, &program);

	projection = glm::ortho(0.f, float(WIDTH - 1), float(HEIGHT - 1), 0.f);

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
		cout << "play" << endl;
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



