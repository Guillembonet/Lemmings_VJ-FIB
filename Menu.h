#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Button.h"
#include <functional>


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
	Texture texPlay, texIns, texExit;
	Button *playButton, *insButton, *exitButton;
	glm::mat4 projection;
};


#endif // _MENU_INCLUDE

