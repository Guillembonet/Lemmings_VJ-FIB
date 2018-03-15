#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include <functional>
#include "Quad.h"


class Menu
{

public:
	Menu();
	~Menu();

	void init(std::function<void()>);
	void update(int deltaTime);
	void render();

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

private:
	void initShaders();

private:
	float currentTime;

	std::function<void()> callback;
	ShaderProgram program;
	Quad *quad;
};


#endif // _MENU_INCLUDE

