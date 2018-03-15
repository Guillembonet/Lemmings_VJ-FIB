#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Menu.h"


Menu::Menu()
{

}

Menu::~Menu()
{

}


void Menu::init(std::function<void()> callback)
{
	this->callback = callback; // Nos guardamos el callback para ejecutarlo cuando lo creamos preciso
	initShaders();
	currentTime = 0.0f;

	quad = Quad::createQuad(-0.4f, 0.0f, 0.8f, 0.3f, program); // Esto vendría a ser el botón de inicio
}

void Menu::update(int deltaTime)
{
	currentTime += deltaTime;
}

void Menu::render()
{
	quad->render();
}

void Menu::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{ 
	if (bLeftButton || bRightButton) callback(); // En caso de que se haga click izquierdo o derecho, llamamos al callback
}

void Menu::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/simple.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/simple.frag");
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
}



