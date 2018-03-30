#ifndef _LEMMING_INCLUDE
#define _LEMMING_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"


// Lemming is basically a Sprite that represents one lemming. As such it has
// all properties it needs to track its movement and collisions.


class Lemming
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	void setMapMask(VariableTexture *mapMask);
	void setExitDoorCoords(int x, int y, int w, int h);

	bool hasLeft();
	
private:
	int collisionFloor(int maxFall);
	bool collision();
	bool inFrontOfExitDoor(int x, int y);
	
private:
	enum LemmingState
	{
		WALKING_LEFT_STATE, WALKING_RIGHT_STATE, FALLING_LEFT_STATE, FALLING_RIGHT_STATE, LEAVING_STATE, OUT_OF_SCENE_STATE
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
};


#endif // _LEMMING_INCLUDE


