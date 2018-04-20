#ifndef _Scene3_INCLUDE
#define _Scene3_INCLUDE


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
#include "finalScreen.h"
#include "Poison.h"
#include "MusicHandler.h"

#define HEIGHT 190
#define WIDTH 848


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene3
{

public:

	enum selectedHab
	{
		BASHER, BLOCKER, BUILDER, CLIMBER, DIGGER, FLOATER, MINER, PAUSED, NONE
	};

	Scene3();
	~Scene3();

	void init(bool *paused, std::function<void()> faster, std::function<void()> slower, std::function<void()> init);
	void initHabilities(std::function<void()> faster, std::function<void()> slower);
	void update(int deltaTime);
	void render();

	void nuke();
	void pause();
	void fasterGen();
	void slowerGen();

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

private:
	void initShaders();
	void applyMask(int mouseX, int mouseY);
	void eraseMask(int mouseX, int mouseY);
	void initLemmings();
	void initDoors();
	void initPoisons();

private:
	Texture colorTexture;
	VariableTexture maskTexture;
	MaskedTexturedQuad *map;
	ShaderProgram simpleTexProgram, maskedTexProgram, overlayProgram;
	float currentTime;
	glm::mat4 projection;
	int lastLemmingGenTime;
	int const MAX_LEMMINGS = 20;
	int lemmingCount;
	int lemXsecs;
	std::vector<Lemming*> lemmings;
	int selectedHab;
	bool *paused, nuked;
	SkyDoor *skyDoor;
	ExitDoor *exitDoor;
	finalScreen fs;
	BottomBox *bb;
	MousePointer *mousePointer;
	SquarePointer *squarePointer;
	int mouseX, mouseY; // last mouse coords
	LadderHandler *ladderHandler;
	vector<int> habsQuant;
	int out;
	int in = 0;

	int offset = 3;
	int leftPos = 70.f;
	int rightPos = 70.f + 320.f;
	float MAX_LEFT_POS;
	float MAX_RIGHT_POS;

	vector<Poison*> poisons;
};


#endif // _Scene3_INCLUDE

