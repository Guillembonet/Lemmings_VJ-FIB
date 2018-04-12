#ifndef _EXPLOSION_INCLUDE
#define _EXPLOSION_INCLUDE
#include "Sprite.h"
#include <vector>
#include "VariableTexture.h"

class Explosion
{

public:
	void init(ShaderProgram *shaderProgram);

	void start(const glm::vec2 &initialPosition);
	void update(int deltaTime);
	void render();

private:

	std::vector<Sprite*> particles;
	std::vector<float> speeds;
	Texture spritesheet;
	glm::vec2 pos0;
	float explosionStartTime;
};


#endif // _EXPLOSION_INCLUDE
