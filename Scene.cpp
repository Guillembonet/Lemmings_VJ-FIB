#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"

#define HABILITIES_NUMBER 4

Scene::Scene()
{
	map = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
}


void Scene::init()
{
	glm::vec2 geom[2] = {glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT))};
	glm::vec2 texCoords[2] = {glm::vec2(120.f / 512.0, 0.f), glm::vec2((120.f + 320.f) / 512.0f, 160.f / 256.0f)};

	initShaders();

	map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, maskedTexProgram);
	colorTexture.loadFromFile("images/fun1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);
	maskTexture.loadFromFile("images/fun1_mask.png", TEXTURE_PIXEL_FORMAT_L);
	maskTexture.setMinFilter(GL_NEAREST);
	maskTexture.setMagFilter(GL_NEAREST);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	Lemming *newLemming = new Lemming();
	newLemming->init(glm::vec2(60, 30), simpleTexProgram);
	newLemming->setMapMask(&maskTexture);

	lemmings.push_back(newLemming);

	initHabilities();
}

unsigned int x = 0;

void Scene::initHabilities()
{
	glm::vec2 texCoords[2] = { glm::vec2(0.0f, 0.f), glm::vec2(1.0f, 1.0f) }; //creo que si pones 1 en los dos coge todo
	
	for (int i = 0; i < HABILITIES_NUMBER; ++i) {
		habilitiesTexs.push_back(new Texture());
		habilitiesTexs[i]->loadFromFile("images/umbrellaLemming.png", TEXTURE_PIXEL_FORMAT_RGBA);
		habilitiesTexs[i]->setMinFilter(GL_NEAREST);
		habilitiesTexs[i]->setMagFilter(GL_NEAREST);
	}

	std::function<void()> callback;

	for (int i = 0; i < HABILITIES_NUMBER; ++i) {
		glm::vec2 geom[2] = { glm::vec2(float(CAMERA_WIDTH - 1) / 2.0f - (25.0f*HABILITIES_NUMBER/2.0f) + i*25.0f, float(CAMERA_HEIGHT - 1) - 25.0f),
			glm::vec2(float(CAMERA_WIDTH - 1) / 2.0f - (25.0f*HABILITIES_NUMBER / 2.0f) + (i+1)*25.0f, float(CAMERA_HEIGHT - 1)) };
		habilities.push_back(Button::createButton(geom, texCoords, callback, habilitiesTexs[i], &overlayProgram));
	}
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;

	int currentTimeSec = currentTime / 1000;
	if ((currentTimeSec % 3 == 0) && currentTimeSec != lastLemmingGenTime && lemmings.size() < MAX_LEMMINGS) {
		lastLemmingGenTime = currentTimeSec;
		Lemming *newLemming = new Lemming();
		newLemming->init(glm::vec2(60, 30), simpleTexProgram);
		newLemming->setMapMask(&maskTexture);

		lemmings.push_back(newLemming);
	}

	for each (Lemming *lem in lemmings)
	{
		lem->update(deltaTime);
	}
}

void Scene::render()
{
	glm::mat4 modelview;

	maskedTexProgram.use();
	maskedTexProgram.setUniformMatrix4f("projection", projection);
	maskedTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	maskedTexProgram.setUniformMatrix4f("modelview", modelview);
	map->render(maskedTexProgram, colorTexture, maskTexture);
	
	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);

	for each (Lemming *lem in lemmings)
	{
		lem->render();
	}

	overlayProgram.use();
	overlayProgram.setUniformMatrix4f("projection", projection);
	overlayProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	overlayProgram.setUniformMatrix4f("modelview", modelview);
	for (Button* h : habilities) {
		h->render();
	}
}

void Scene::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	if(bLeftButton)
		eraseMask(mouseX, mouseY);
	else if(bRightButton)
		applyMask(mouseX, mouseY);
}

void Scene::eraseMask(int mouseX, int mouseY)
{
	int posX, posY;
	
	// Transform from mouse coordinates to map coordinates
	//   The map is enlarged 3 times and displaced 120 pixels
	posX = mouseX/3 + 120;
	posY = mouseY/3;

	for(int y=max(0, posY-3); y<=min(maskTexture.height()-1, posY+3); y++)
		for(int x=max(0, posX-3); x<=min(maskTexture.width()-1, posX+3); x++)
			maskTexture.setPixel(x, y, 0);
}

void Scene::applyMask(int mouseX, int mouseY)
{
	int posX, posY;
	
	// Transform from mouse coordinates to map coordinates
	//   The map is enlarged 3 times and displaced 120 pixels
	posX = mouseX/3 + 120;
	posY = mouseY/3;

	for(int y=max(0, posY-3); y<=min(maskTexture.height()-1, posY+3); y++)
		for(int x=max(0, posX-3); x<=min(maskTexture.width()-1, posX+3); x++)
			maskTexture.setPixel(x, y, 255);
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleTexProgram.init();
	simpleTexProgram.addShader(vShader);
	simpleTexProgram.addShader(fShader);
	simpleTexProgram.link();
	if(!simpleTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleTexProgram.log() << endl << endl;
	}
	simpleTexProgram.bindFragmentOutput("outColor");

	overlayProgram.init();
	overlayProgram.addShader(vShader);
	overlayProgram.addShader(fShader);
	overlayProgram.link();
	if (!overlayProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << overlayProgram.log() << endl << endl;
	}
	overlayProgram.bindFragmentOutput("outColor");

	vShader.free();
	fShader.free();

	vShader.initFromFile(VERTEX_SHADER, "shaders/maskedTexture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/maskedTexture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	maskedTexProgram.init();
	maskedTexProgram.addShader(vShader);
	maskedTexProgram.addShader(fShader);
	maskedTexProgram.link();
	if(!maskedTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << maskedTexProgram.log() << endl << endl;
	}
	maskedTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



