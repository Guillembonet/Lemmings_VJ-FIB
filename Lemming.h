#ifndef _LEMMING_INCLUDE
#define _LEMMING_INCLUDE

#include <set>
#include "Sprite.h"
#include "VariableTexture.h"


// Lemming is basically a Sprite that represents one lemming. As such it has
// all properties it needs to track its movement and collisions.


class Lemming
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);
	void update(int deltaTime, vector<glm::vec2> &blockers);
	void render();
	
	void setMapMask(VariableTexture *mapMask);
	void setExitDoorCoords(int x, int y, int w, int h);

	bool hasLeft();
	bool isBlocker();
	glm::vec2 getPosition();
	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);
	
private:
	bool found(vector<glm::vec2> &vector, glm::vec2 element);
	int collisionFloor(int maxFall);
	int Lemming::collisionFloorWithCoords(int maxFall, int x, int y);
	bool collision();
	bool inFrontOfExitDoor(int x, int y);
	
private:
	bool isLemmingSelected(int x, int y, int i, int j);

	enum LemmingState
	{
		WALKING_LEFT_STATE, WALKING_RIGHT_STATE, FALLING_LEFT_STATE, FALLING_RIGHT_STATE, LEAVING_STATE, OUT_OF_SCENE_STATE,
		DIGGING_STATE, BASHING_LEFT_STATE, BASHING_RIGHT_STATE, BLOCKING_STATE
	};

	enum Side {
		RIGHT, LEFT
	};

	struct Box {
		int x;
		int y;
		int width;
		int height;
	};

	LemmingState state;
	Texture spritesheet;
	Sprite *sprite;
	VariableTexture *mask;
	Box exitDoor;

	Side side;
};


#endif // _LEMMING_INCLUDE


