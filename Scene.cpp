#include <iostream>
#include <cmath>
#include <algorithm>
#include <set>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"

#define HABILITIES_NUMBER 4

Scene::Scene()
{
	paused = false;
	map = NULL;
	nuked = false;
	selectedHab = NONE;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
}


void Scene::init(bool *paused, std::function<void()> faster, std::function<void()> slower, std::function<void()> init)
{
	lastLemmingGenTime = -1;
	out = 0;
	in = 0;
	nuked = false;
	lemmings.clear();
	lemmingCount = 0;
	fs.init(init);
	this->paused = paused;
	glm::vec2 geom[2] = {glm::vec2(0.f, 0.f), glm::vec2(float(WIDTH), float(HEIGHT))};
	glm::vec2 texCoords[2] = {glm::vec2(0.f / 512.0, 0.f), glm::vec2(512.f / 512.0f, 190.f / 256.0f)};

	lemXsecs = 3;

	initShaders();

	map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, maskedTexProgram);
	colorTexture.loadFromFile("images/fun1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);
	maskTexture.loadFromFile("images/fun1_mask.png", TEXTURE_PIXEL_FORMAT_L);
	maskTexture.setMinFilter(GL_NEAREST);
	maskTexture.setMagFilter(GL_NEAREST);

	projection = glm::ortho(120.f, 320.f + 120.f, float(HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	initDoors();
	initHabilities(faster, slower);

	mousePointer = new MousePointer();
	mousePointer->init(glm::vec2(200, 25), simpleTexProgram, &leftPos, &rightPos);

	squarePointer = new SquarePointer();
	squarePointer->init(glm::vec2(80, 25), simpleTexProgram);

	ladderHandler = new LadderHandler();
	ladderHandler->init(&simpleTexProgram);

	// black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Hidden cursor
	glutSetCursor(GLUT_CURSOR_NONE);
}

unsigned int x = 0;

void Scene::initDoors() {
	skyDoor = new SkyDoor();
	skyDoor->init(glm::vec2(80 + 120.f, 25), simpleTexProgram, "images/skyDoor.png");

	exitDoor = new ExitDoor();
	exitDoor->init(glm::vec2(225 + 120.f, 106), simpleTexProgram, "images/exitDoor.png");
}

void Scene::initHabilities(std::function<void()> faster, std::function<void()> slower)
{
	std::function<void()> callback;

	habsQuant = { 5, 0, 6, 7, 6, 4, 5 };

	bb = new BottomBox();
	bb->init(MAX_LEMMINGS, &in, &out, &habsQuant, &selectedHab, &overlayProgram, std::bind(&Scene::nuke, this), std::bind(&Scene::pause, this), faster, slower, std::bind(&Scene::fasterGen, this), std::bind(&Scene::slowerGen, this));
}

void Scene::nuke() {
	for each (Lemming *lem in lemmings)
	{
		lem->nuke();
	}
	nuked = true;
}

void Scene::pause() {
	*paused = !*paused;
	if (!*paused)
		selectedHab = NONE;
}

void Scene::fasterGen() {
	if (lemXsecs > 1)
		--lemXsecs;
}

void Scene::slowerGen() {
	if (lemXsecs < 6)
		++lemXsecs;
}

void Scene::update(int deltaTime)
{
	/* Move scene */
	//std::cout << mouseX << std::endl;
	if (mouseX <= 30 && leftPos > 0) {
		//std::cout << "left" << std::endl;
		leftPos = leftPos - offset;
		if (leftPos < 0) leftPos = 0;
		rightPos = rightPos - offset;
		projection = glm::ortho(float(leftPos), float(rightPos), float(HEIGHT - 1), 0.f);
		mousePointer->update(deltaTime);
	}
	else if (mouseX >= 960 - 30 && rightPos < 512) {
		//std::cout << "right" << std::endl;
		leftPos = leftPos + offset;
		rightPos = rightPos + offset;
		if (rightPos > 512) rightPos = 512;
		projection = glm::ortho(float(leftPos), float(rightPos), float(HEIGHT - 1), 0.f);
		mousePointer->update(deltaTime);
	}

	/*             */
	currentTime += deltaTime;

	int currentTimeSec = currentTime / 1000;
	if (skyDoor->isDoorOpen() && (currentTimeSec % lemXsecs == 0) && currentTimeSec != lastLemmingGenTime && lemmingCount < MAX_LEMMINGS && !nuked) {
		lastLemmingGenTime = currentTimeSec;
		Lemming *newLemming = new Lemming();
		newLemming->init(&in, &habsQuant, glm::vec2(90 + 120.f, 27), simpleTexProgram);
		newLemming->setLadderHandler(ladderHandler);
		newLemming->setMapMask(&maskTexture);
		newLemming->setExitDoorCoords(233 + 120.f, 117, 4, 5);

		lemmings.push_back(newLemming);

		lemmingCount++;
	}

	vector<glm::vec2> blockers;

	for (int i = 0; i < lemmings.size(); i++) {
		if (lemmings[i]->isBlocker())
			blockers.push_back(lemmings[i]->getPosition());
	}

	bool isThereALemmSelected = false;
	for (int i = 0; i < lemmings.size(); i++) {
		lemmings[i]->update(deltaTime, blockers);
		if (lemmings[i]->hasLeft()) {
			lemmings.erase(lemmings.begin() + i);
			i--;
		}
		else if (lemmings[i]->isLemmingSelected(mousePointer->getPosition().x, mousePointer->getPosition().y)) {
			int x = lemmings[i]->getPosition().x;
			int y = lemmings[i]->getPosition().y;
			squarePointer->display(glm::vec2(x + 3, y + 5)); // 3 & 5 are just consts for center lem's

			isThereALemmSelected = true;
		}
		if (!isThereALemmSelected) squarePointer->hidde();
	}

	skyDoor->update(deltaTime);
	exitDoor->update(deltaTime);
	squarePointer->update(deltaTime);
	bb->update(deltaTime);

	if (lemmings.size() == 0 && (currentTime/1000 > 5)) {
		std::cout << "Fin de la escena" << std::endl;
	}
}

void Scene::render()
{
	if (out == 0 && currentTime > 8000) {
		fs.set(out, in, MAX_LEMMINGS);
		fs.render();
	}
	else {
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

		exitDoor->render();
		skyDoor->render();
		ladderHandler->render();
		out = 0;
		for each (Lemming *lem in lemmings)
		{
			simpleTexProgram.use();
			simpleTexProgram.setUniformMatrix4f("projection", projection);
			simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			modelview = glm::mat4(1.0f);
			simpleTexProgram.setUniformMatrix4f("modelview", modelview);
			if (lem->render(leftPos))
				++out;
		}
		bb->render();

		simpleTexProgram.use();
		simpleTexProgram.setUniformMatrix4f("projection", projection);
		simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		modelview = glm::mat4(1.0f);
		simpleTexProgram.setUniformMatrix4f("modelview", modelview);

		mousePointer->render();
		squarePointer->render();
	}
}

void Scene::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	/*if (bLeftButton)
		eraseMask(mouseX, mouseY);
	else if(bRightButton)
		applyMask(mouseX, mouseY);*/

	if (out == 0 && currentTime > 5000) {
		fs.mouseMoved(mouseX, mouseY, bLeftButton, bRightButton);
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	}

	mousePointer->mouseMoved(mouseX, mouseY, bLeftButton, bRightButton);
	auto pos = mousePointer->getPosition();
	for each (Lemming* lem in lemmings)
	{
		lem->mouseMoved(pos.x, pos.y, bLeftButton, bRightButton, selectedHab);
	}

	bb->mouseMoved(mouseX, mouseY, bLeftButton, bRightButton);

	// We update mouse coords
	this->mouseX = mouseX;
	this->mouseY = mouseY;
}

void Scene::eraseMask(int mouseX, int mouseY)
{
	maskTexture.eraseMask(mouseX, mouseY);
}

void Scene::applyMask(int mouseX, int mouseY)
{
	maskTexture.applyMask(mouseX, mouseY);
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



