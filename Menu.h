#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TexturedQuad.h"
#include "Button.h"
#include "MenuButton.h"
#include <functional>
#include "MusicHandler.h"

#define HEIGHT 190
#define WIDTH 512

class Menu
{

public:
	enum state { BASIC, LEVELS, INSTRUCTIONS };

	Menu();
	~Menu();

	void init(vector<std::function<void()>>, std::function<void()>);
	void update(int deltaTime);
	void render();

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

	void setState(state newState);

private:
	void initShaders();

private:
	float currentTime;

	ShaderProgram program;
	Texture texPlay, texIns, texExit, texBg, texInsBg;
	//Button *playButton, *insButton, *exitButton;
	MenuButton  *playButton, *insButton, *exitButton, *level1, *level2, *level3, *back;
	glm::mat4 projection;
	TexturedQuad *bg;
	state currentState = BASIC;
};


#endif // _MENU_INCLUDE

