#include <iostream>
#include <cmath>
#include <algorithm>
#include <set>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene3.h"

#define HABILITIES_NUMBER 4

Scene3::Scene3()
{
	paused = false;
	map = NULL;
	nuked = false;
	selectedHab = NONE;
}

Scene3::~Scene3()
{
	if (map != NULL)
		delete map;
}


void Scene3::init(bool *paused, std::function<void()> faster, std::function<void()> slower, std::function<void()> init)
{
	lastLemmingGenTime = -1;
	nuked = false;
	out = 0;
	in = 0;
	lemmingCount = 0;
	lemmings.clear();
	fs.init(init);
	this->paused = paused;
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(WIDTH), float(HEIGHT)) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f / 512.0, 0.f), glm::vec2(512.f / 512.0f, 190.f / 256.0f) };

	lemXsecs = 3;

	initShaders();

	map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, maskedTexProgram);
	colorTexture.loadFromFile("images/fun5.png", TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);
	maskTexture.loadFromFile("images/fun5_mask.png", TEXTURE_PIXEL_FORMAT_L);
	maskTexture.setMinFilter(GL_NEAREST);
	maskTexture.setMagFilter(GL_NEAREST);

	projection = glm::ortho(float(leftPos), float(rightPos), float(HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	initDoors();
	initHabilities(faster, slower);
	initPoisons();

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

void Scene3::initPoisons() {
	for (int i = 0; i < 4; i++) {
		Poison *p = new Poison();
		p->init(glm::vec2(19 + 120.f + i * 40, 140), simpleTexProgram, "images/icePoison.png");
		poisons.push_back(p);
	}
}

void Scene3::initDoors() {
	skyDoor = new SkyDoor();
	skyDoor->init(glm::vec2(20 + 120.f, 25), simpleTexProgram, "images/skyDoor3.png");

	exitDoor = new ExitDoor();
	exitDoor->init(glm::vec2(550 + 130.f, 106), simpleTexProgram, "images/exitDoor3.png");
}

void Scene3::initHabilities(std::function<void()> faster, std::function<void()> slower)
{
	std::function<void()> callback;

	habsQuant = { 5, 0, 6, 7, 6, 4, 5 };

	bb = new BottomBox();
	bb->init(MAX_LEMMINGS, &in, &out, &habsQuant, &selectedHab, &overlayProgram, std::bind(&Scene3::nuke, this), std::bind(&Scene3::pause, this), faster, slower, std::bind(&Scene3::fasterGen, this), std::bind(&Scene3::slowerGen, this));
}

void Scene3::nuke() {
	MusicHandler::play("songs/moab.mp3", false);
	for each (Lemming *lem in lemmings)
	{
		lem->nuke();
	}
	nuked = true;
}

void Scene3::pause() {
	*paused = !*paused;
	if (!*paused)
		selectedHab = NONE;
}

void Scene3::fasterGen() {
	if (lemXsecs > 1)
		--lemXsecs;
}

void Scene3::slowerGen() {
	if (lemXsecs < 6)
		++lemXsecs;
}

void Scene3::update(int deltaTime)
{
	for each (Poison *p in poisons)
	{
		p->update(deltaTime);
	}

	if (mouseX <= 30 && leftPos > 0) {
		//std::cout << "left" << std::endl;
		leftPos = leftPos - offset;
		if (leftPos < 0) leftPos = 0;
		rightPos = rightPos - offset;
		projection = glm::ortho(float(leftPos), float(rightPos), float(HEIGHT - 1), 0.f);
		mousePointer->update(deltaTime);
	}
	else if (mouseX >= 960 - 30 && rightPos < 848) {
		//std::cout << "right" << std::endl;
		leftPos = leftPos + offset;
		rightPos = rightPos + offset;
		if (rightPos > 848) rightPos = 848;
		projection = glm::ortho(float(leftPos), float(rightPos), float(HEIGHT - 1), 0.f);
		mousePointer->update(deltaTime);
	}

	/*             */
	currentTime += deltaTime;

	int currentTimeSec = currentTime / 1000;
	if (skyDoor->isDoorOpen() && (currentTimeSec % lemXsecs == 0) && currentTimeSec != lastLemmingGenTime && lemmingCount < MAX_LEMMINGS && !nuked) {
		lastLemmingGenTime = currentTimeSec;
		Lemming *newLemming = new Lemming();
		newLemming->init(&in, &habsQuant, glm::vec2(30 + 120.f, 27), simpleTexProgram);
		newLemming->setLadderHandler(ladderHandler);
		newLemming->setMapMask(&maskTexture);
		newLemming->setExitDoorCoords(245 + 120.f, 23, 4, 5);

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

	if (lemmings.size() == 0 && (currentTime / 1000 > 5)) {
		std::cout << "Fin de la escena" << std::endl;
	}
}

void Scene3::render()
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
			if (lem->render())
				++out;
		}
		for each(Poison *po in poisons) {
			simpleTexProgram.use();
			simpleTexProgram.setUniformMatrix4f("projection", projection);
			simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
			modelview = glm::mat4(1.0f);
			simpleTexProgram.setUniformMatrix4f("modelview", modelview);
			po->render();
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

void Scene3::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	/*if (bLeftButton)
	eraseMask(mouseX, mouseY);
	else if(bRightButton)
	applyMask(mouseX, mouseY);*/

	if (out == 0 && currentTime > 5000) {
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		fs.mouseMoved(mouseX, mouseY, bLeftButton, bRightButton);
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

void Scene3::eraseMask(int mouseX, int mouseY)
{
	maskTexture.eraseMask(mouseX, mouseY);
}

void Scene3::applyMask(int mouseX, int mouseY)
{
	maskTexture.applyMask(mouseX, mouseY);
}

void Scene3::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleTexProgram.init();
	simpleTexProgram.addShader(vShader);
	simpleTexProgram.addShader(fShader);
	simpleTexProgram.link();
	if (!simpleTexProgram.isLinked())
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
	maskedTexProgram.init();
	maskedTexProgram.addShader(vShader);
	maskedTexProgram.addShader(fShader);
	maskedTexProgram.link();
	if (!maskedTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << maskedTexProgram.log() << endl << endl;
	}
	maskedTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}