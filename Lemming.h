#ifndef _LEMMING_INCLUDE
#define _LEMMING_INCLUDE
class Scene;
#include <set>
#include "Sprite.h"
#include "Text.h"
#include "Explosion.h"
#include "VariableTexture.h"
#include "LadderHandler.h"


// Lemming is basically a Sprite that represents one lemming. As such it has
// all properties it needs to track its movement and collisions.


class Lemming
{

public:

	enum selectedHab
	{
		BASHER, BLOCKER, BUILDER, CLIMBER, DIGGER, FLOATER, MINER, PAUSED, NONE
	};

	void init(int *in, vector<int> *habs, const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);
	void update(int deltaTime, vector<glm::vec2> &blockers);
	bool render();
	
	void setMapMask(VariableTexture *mapMask);
	void setExitDoorCoords(int x, int y, int w, int h);
	void setLadderHandler(LadderHandler *ladderHandler);

	bool hasLeft();
	bool isBlocker();
	glm::vec2 getPosition();
	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton, int habID);
	bool isLemmingSelected(int i, int j);
	void nuke();
	
private:
	bool found(vector<glm::vec2> &vector, glm::vec2 element);
	bool wall();
	bool wallIsEnding();
	int collisionFloor(int maxFall);
	int Lemming::collisionFloorWithCoords(int maxFall, int x, int y);
	bool collision();
	bool inFrontOfExitDoor(int x, int y);
	bool imSelected(int x, int y, int i, int j);
	
private:

	enum LemmingState
	{
		WALKING_LEFT_STATE, WALKING_RIGHT_STATE, FALLING_LEFT_STATE, FALLING_RIGHT_STATE, LEAVING_STATE, OUT_OF_SCENE_STATE,
		DIGGING_STATE, BASHING_LEFT_STATE, BASHING_RIGHT_STATE, BLOCKING_STATE, CLIMBING_RIGHT_STATE, CLIMBING_LEFT_STATE, EXPLODING_STATE, 
		BUILDING_LEFT_STATE, BUILDING_RIGHT_STATE
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

	bool climbing, endingclimb, exploding, exploded;
	LemmingState state;
	Texture spritesheet;
	Sprite *sprite;
	VariableTexture *mask;
	Box exitDoor;
	Text explodingNumber;
	long explodeTime;
	Explosion explosion;
	Side side;
	float currentTime;
	vector<int> *habsQuant;
	int *in;

	LadderHandler *ladderHandler;
	int ladderCount = 0;
};


#endif // _LEMMING_INCLUDE


