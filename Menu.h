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
	enum state { BASIC, LEVELS };

	Menu();
	~Menu();

	void init(std::function<void()>, std::function<void()>);
	void update(int deltaTime);
	void render();

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

	void setState(state newState);

private:
	void initShaders();

private:
	float currentTime;

	ShaderProgram program;
	Texture texPlay, texIns, texExit, texBg;
	//Button *playButton, *insButton, *exitButton;
	MenuButton  *playButton, *insButton, *exitButton, *level1, *level2, *level3;
	glm::mat4 projection;
	TexturedQuad *bg;
	state currentState = BASIC;
};


#endif // _MENU_INCLUDE

