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


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void initHabilities();
	void update(int deltaTime);
	void render();
	
	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

private:
	void initShaders();
	void eraseMask(int mouseX, int mouseY);
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
	int const MAX_LEMMINGS = 1;
	int lemmingCount = 0;
	std::vector<Lemming*> lemmings;
	string selectedHab;

	SkyDoor *skyDoor;
	ExitDoor *exitDoor;
	BottomBox *bb;
	MousePointer *mousePointer;
};


#endif // _SCENE_INCLUDE

