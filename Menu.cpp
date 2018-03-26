#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Menu.h"


Menu::Menu()
{
	button = NULL;
}

Menu::~Menu()
{
	if (button != NULL)
		delete button;
}


void Menu::init(std::function<void()> callback)
{
	currentTime = 0.0f;

	//posicion del boton
	glm::vec2 geom[2] = { glm::vec2(float(CAMERA_WIDTH)/4.0f, float(CAMERA_HEIGHT)/4.0f), glm::vec2(float(CAMERA_WIDTH) * 3.0f / 4.0f, float(CAMERA_HEIGHT) / 2.0f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.0f, 0.f), glm::vec2(1.0f, 1.0f) }; //creo que si pones 1 en los dos coge todo

	initShaders();

	tex.loadFromFile("images/play_button.png", TEXTURE_PIXEL_FORMAT_RGBA);
	tex.setMinFilter(GL_NEAREST);
	tex.setMagFilter(GL_NEAREST);
	button = Button::createButton(geom, texCoords, callback, &tex, &program);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);

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
	button->render();
}

void Menu::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{ 
	button->mouseMoved(mouseX, mouseY, bLeftButton, bRightButton);
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



