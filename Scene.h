#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Lemming.h"
#include "Button.h"
#include "SkyDoor.h"
#include "ExitDoor.h"
#include "BottomBox.h"
#include "MousePointer.h"
#include "SquarePointer.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init(bool *paused, std::function<void()> faster, std::function<void()> slower);
	void initHabilities(std::function<void()> faster, std::function<void()> slower);
	void update(int deltaTime);
	void render();
	void nuke();
	void pause();

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);
	void eraseMask(int mouseX, int mouseY);

private:
	void initShaders();
	void applyMask(int mouseX, int mouseY);
	void initLemmings();
	void initDoors();

private:
	Texture colorTexture;
	VariableTexture maskTexture;
	MaskedTexturedQuad *map;
	ShaderProgram simpleTexProgram, maskedTexProgram, overlayProgram;
	float currentTime;
	glm::mat4 projection;
	int lastLemmingGenTime;
	int const MAX_LEMMINGS = 10;
	int lemmingCount = 0;
	std::vector<Lemming*> lemmings;
	string selectedHab;
	bool *paused, nuked;
	SkyDoor *skyDoor;
	ExitDoor *exitDoor;
	BottomBox *bb;
	MousePointer *mousePointer;
	SquarePointer *squarePointer;
	int mouseX, mouseY; // last mouse coords
};


#endif // _SCENE_INCLUDE

