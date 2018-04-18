#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TexturedQuad.h"
#include "Button.h"
#include "MenuButton.h"
#include <functional>

#define HEIGHT 190
#define WIDTH 512

class Menu
{

public:
	Menu();
	~Menu();

	void init(std::function<void()>, std::function<void()>);
	void update(int deltaTime);
	void render();

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

private:
	void initShaders();

private:
	float currentTime;

	ShaderProgram program;
	Texture texPlay, texIns, texExit, texBg;
	//Button *playButton, *insButton, *exitButton;
	MenuButton  *playButton, *insButton, *exitButton;
	glm::mat4 projection;
	TexturedQuad *bg;
};


#endif // _MENU_INCLUDE

