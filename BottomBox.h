#ifndef _BOTTOMBOX_INCLUDE
#define _BOTTOMBOX_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Button.h"
#include "Text.h"
#include <functional>
#include <vector>

#define HEIGHT 190
#define WIDTH 320

class BottomBox {

public:
	BottomBox();
	~BottomBox();

	void init(int *lemmCount, vector<int> *habs, int *selectedHab, ShaderProgram *overlayProgram, std::function<void()> nuke, std::function<void()> pause, std::function<void()> faster, std::function<void()> slower, std::function<void()> fasterGen, std::function<void()> slowerGen);

	void render();
	void update(int deltaTime);

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

	void selectHab(int hab);
	void callAndSetHab(std::function<void()> call, int hab);

private:

	std::vector<Button*> habilities;
	std::vector<Text*> habsNums;
	std::vector<Texture*> habilitiesTexs;
	ShaderProgram *overlayProgram;
	glm::mat4 projection;
	glm::vec2 position[2];
	bool focus;
	int *selectedHab;
	vector<int> *habsQuant;
	float currentTime;
	Text time, in, out;
	int *outNum;
};

#endif //_BOTTOMBOX_INCLUDE
