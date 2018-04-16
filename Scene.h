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

#define HEIGHT 190
#define WIDTH 512


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:

	enum selectedHab
	{
		BASHER, BLOCKER, BUILDER, CLIMBER, DIGGER, FLOATER, MINER, PAUSED, NONE
	};

	Scene();
	~Scene();

	void init(bool *paused, std::function<void()> faster, std::function<void()> slower);
	void initHabilities(std::function<void()> faster, std::function<void()> slower);
	void update(int deltaTime);
	void render();

	void nuke();
	void pause();
	void fasterGen();
	void slowerGen();

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
	int lemXsecs;
	std::vector<Lemming*> lemmings;
	int selectedHab;
	bool *paused, nuked;
	SkyDoor *skyDoor;
	ExitDoor *exitDoor;
	BottomBox *bb;
	MousePointer *mousePointer;
	SquarePointer *squarePointer;
	int mouseX, mouseY; // last mouse coords
	LadderHandler *ladderHandler;

	int offset = 3;
	int leftPos = 120.f;
	int rightPos = 120.f + 320.f;
	float MAX_LEFT_POS;
	float MAX_RIGHT_POS;
};


#endif // _SCENE_INCLUDE

