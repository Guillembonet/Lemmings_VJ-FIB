#include <iostream>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>
#include "finalScreen.h"


finalScreen::finalScreen() {}

finalScreen::~finalScreen() {}

void finalScreen::init(std::function<void()> backFunc)
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

	back = new MenuButton();
	back->init(glm::vec2(960 * 4 / 5 - 100, 480), program, "images/back_button.png");
	back->attachCallback(backFunc);

	projection = glm::ortho(0.f, float(960 - 1), float(642 - 1), 0.f);

	MusicHandler::play("songs/background.mp3", true);
	MusicHandler::setVolume("songs/background.mp3", 120);

	if (!winorlose.init("fonts/OpenSans-Regular.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;

	if (!stats.init("fonts/OpenSans-Regular.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
}

void finalScreen::set(int nout, int nin, int ntotalLemms) {
	this->in = nin;
	this->out = nout;
	this->totalLemms = ntotalLemms;
}

void finalScreen::update(int deltaTime)
{
	currentTime += deltaTime;
}

void finalScreen::render()
{
	glm::mat4 modelview;

	program.use();
	program.setUniformMatrix4f("projection", projection);
	program.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	program.setUniformMatrix4f("modelview", modelview);
	bg->render(texBg);

	if ((float)in / (float)totalLemms*100.0f >= 95.0f) {
		winorlose.render("YOU WIN!",
			glm::vec2(WIDTH/2.0f, HEIGHT), 100, glm::vec4(0, 0, 0, 1));
	}
	else {
		winorlose.render("YOU LOSE!",
			glm::vec2(WIDTH / 2.0f, HEIGHT), 100, glm::vec4(0, 0, 0, 1));
	}
	stats.render(static_cast<ostringstream*>(&(ostringstream() <<"You saved " << in << " out of " << totalLemms << " (" << (float)in / (float)totalLemms*100.0f << "%) Lemmings"))->str(),
		glm::vec2(WIDTH*2.0f / 3.0f, HEIGHT * 2.0f), 20, glm::vec4(0, 0, 0, 1));

	program.use();
	program.setUniformMatrix4f("projection", projection);
	program.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	program.setUniformMatrix4f("modelview", modelview);

	if (back->isMouseOver())
		program.setUniform4f("color", 0.5f, 1.0f, 0.5f, 1.0f);
	else
		program.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	back->render();
}

void finalScreen::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	back->mouseMoved(mouseX, mouseY, bLeftButton, bRightButton);
}

void finalScreen::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/menu.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/menu.frag");
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



