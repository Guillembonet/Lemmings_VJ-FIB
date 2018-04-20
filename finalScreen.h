#ifndef _FINALSCREEN_INCLUDE
#define _FINALSCREEN_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TexturedQuad.h"
#include "Button.h"
#include "Text.h"
#include "MenuButton.h"
#include <functional>
#include "MusicHandler.h"

#define HEIGHT 190
#define WIDTH 512

class finalScreen
{

public:

	finalScreen();
	~finalScreen();

	void init(std::function<void()>);
	void set(int nin, int nout, int ntotalLemms);
	void update(int deltaTime);
	void render();

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

private:
	void initShaders();

private:
	float currentTime;

	ShaderProgram program;
	Texture texBg;
	MenuButton *back;
	glm::mat4 projection;
	TexturedQuad *bg;
	int in, out, totalLemms;
	Text winorlose, stats;
};


#endif // _FINALSCREEN_INCLUDE

