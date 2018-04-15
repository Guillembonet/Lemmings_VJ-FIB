#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::callback(){ // En caso de que esta función sea llamada, se iniciará la escena del juego y se cambiará el estado
	std::function<void()> faster = std::bind(&Game::faster, this);
	std::function<void()> slower = std::bind(&Game::slower, this);
	scene.init(&paused, faster, slower);
	state = PLAYING;
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

	std::function<void()> functionObject = std::bind(&Game::callback, this); // Creamos obj. function y le enlazamos nuestro callback
	std::function<void()> exitFunction = std::bind(&Game::end_game, this);
	menu.init(functionObject, exitFunction); // Iniciamos el menú pasándole el obj. function que se podrá ejecutar como si de una función se tratase
}

bool Game::update(int deltaTime)
{
	switch (state)
	{
		case MENU:
			menu.update(deltaTime);
			break;
		case PLAYING:
			if (!paused)
				scene.update(deltaTime);
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
	case PLAYING:
		scene.render();
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
	case PLAYING:
		scene.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
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
		case PLAYING:
			scene.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
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
		case PLAYING:
			scene.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
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





