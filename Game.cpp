#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::callback1(){ // En caso de que esta función sea llamada, se iniciará la escena del juego y se cambiará el estado
	std::function<void()> faster = std::bind(&Game::faster, this);
	std::function<void()> slower = std::bind(&Game::slower, this);

	scene1.init(&paused, faster, slower);
	state = PLAYING_SCENE1;

	MusicHandler::pause("songs/background.mp3");
	MusicHandler::play("songs/scene1.mp3", true);
	MusicHandler::setVolume("songs/scene1.mp3", 130);
}

void Game::callback2() { // En caso de que esta función sea llamada, se iniciará la escena del juego y se cambiará el estado
	std::function<void()> faster = std::bind(&Game::faster, this);
	std::function<void()> slower = std::bind(&Game::slower, this);

	scene2.init(&paused, faster, slower);
	state = PLAYING_SCENE2;

	MusicHandler::pause("songs/background.mp3");
	MusicHandler::play("songs/scene2.mp3", true);
	MusicHandler::setVolume("songs/scene2.mp3", 130);
}

void Game::callback3() { // En caso de que esta función sea llamada, se iniciará la escena del juego y se cambiará el estado
	std::function<void()> faster = std::bind(&Game::faster, this);
	std::function<void()> slower = std::bind(&Game::slower, this);

	scene3.init(&paused, faster, slower);
	state = PLAYING_SCENE3;

	MusicHandler::pause("songs/background.mp3");
	MusicHandler::play("songs/scene3.mp3", true);
	MusicHandler::setVolume("songs/scene3.mp3", 130);
}

void Game::end_game() {
	bPlay = false;
}

void Game::init(float *speed)
{
	bPlay = true;
	this->speed = speed;
	paused = false;
	bLeftMouse = bRightMouse = false;
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	state = MENU; // Inicialmente tendremos el menú

	std::function<void()> call1 = std::bind(&Game::callback1, this); // Creamos obj. function y le enlazamos nuestro callback
	std::function<void()> call2 = std::bind(&Game::callback2, this);
	std::function<void()> call3 = std::bind(&Game::callback3, this);
	vector<function<void()>> calls;
	calls.push_back(call1);
	calls.push_back(call2);
	calls.push_back(call3);
	std::function<void()> exitFunction = std::bind(&Game::end_game, this);
	menu.init(calls, exitFunction); // Iniciamos el menú pasándole el obj. function que se podrá ejecutar como si de una función se tratase
}

bool Game::update(int deltaTime)
{
	switch (state)
	{
		case MENU:
			menu.update(deltaTime);
			break;
		case PLAYING_SCENE1:
			if (!paused)
				scene1.update(deltaTime);
			break;
		case PLAYING_SCENE2:
			if (!paused)
				scene2.update(deltaTime);
			break;
		case PLAYING_SCENE3:
			if (!paused)
				scene3.update(deltaTime);
			break;
		default:
			break;
	}

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	switch (state)
	{
	case MENU:
		menu.render();
		break;
	case PLAYING_SCENE1:
		scene1.render();
		break;
	case PLAYING_SCENE2:
		scene2.render();
		break;
	case PLAYING_SCENE3:
		scene3.render();
		break;
	default:
		break;
	}
}

void Game::slower() {
	if (*speed > 1.0f)
		*speed -= 0.2f;
	else if (*speed > 0.5f)
		*speed -= 0.1f;
}

void Game::faster() {
	if (*speed < 1.0f)
		*speed += 0.1f;
	else if (*speed < 2.6f)
		*speed += 0.2f;
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
	mouseX = x;
	mouseY = y;
	//std::cout << mouseX << std::endl;
	switch (state)
	{
	case MENU:
		menu.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
		break;
	case PLAYING_SCENE1:
		scene1.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
		break;
	case PLAYING_SCENE2:
		scene2.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
		break;
	case PLAYING_SCENE3:
		scene3.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
		break;
	default:
		break;
	}

}

void Game::mousePress(int button)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		bLeftMouse = true;

		switch (state)
		{
		case MENU:
			menu.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
			break;
		case PLAYING_SCENE1:
			scene1.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
			break;
		case PLAYING_SCENE2:
			scene2.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
			break;
		case PLAYING_SCENE3:
			scene3.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
			break;
		default:
			break;
		}
	}
	else if(button == GLUT_RIGHT_BUTTON)
	{
		bRightMouse = true;

		switch (state)
		{
		case MENU:
			menu.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
			break;
		case PLAYING_SCENE1:
			scene1.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
			break;
		case PLAYING_SCENE2:
			scene2.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
			break;
		case PLAYING_SCENE3:
			scene3.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
			break;
		default:
			break;
		}
	}
}

void Game::mouseRelease(int button)
{
	if(button == GLUT_LEFT_BUTTON)
		bLeftMouse = false;
	else if(button == GLUT_RIGHT_BUTTON)
		bRightMouse = false;
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





