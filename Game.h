#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"
#include "Scene2.h"
#include "Menu.h"
#include "MusicHandler.h"
#include <functional>
#include <iostream>


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

public:
	Game() {}
	
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init(float *speed);
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);

	void callback1();
	void callback2();
	void end_game();

	void faster();
	void slower();
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

private:
	bool bPlay;                       // Continue to play game?
	Scene scene1;
	Scene2 scene2;                      // Scene to render
	//Scene3 scene3;
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
	int mouseX, mouseY;               // Mouse position
	bool bLeftMouse, bRightMouse;     // Mouse button states
	bool paused;
	enum STATE{
		MENU, PLAYING_SCENE1, PLAYING_SCENE2
	};

	STATE state;
	Menu menu;
	float *speed;
};


#endif // _GAME_INCLUDE


