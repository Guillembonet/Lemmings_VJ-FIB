#ifndef _BOTTOMBOX_INCLUDE
#define _BOTTOMBOX_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Button.h"
#include "Text.h"
#include <functional>
#include <vector>

class BottomBox {

public:

	BottomBox();
	~BottomBox();

	void init(string *selectedHab, ShaderProgram *overlayProgram, std::function<void()> nuke, std::function<void()> pause);

	void render();

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

	void selectHab(string hab);
	void callAndSetHab(std::function<void()> call, string hab);

private:
	std::vector<Button*> habilities;
	std::vector<int> habilitiesQuant;
	std::vector<Text*> habsNums;
	std::vector<Texture*> habilitiesTexs;
	ShaderProgram *overlayProgram;
	glm::mat4 projection;
	glm::vec2 position[2];
	bool focus;
	string *selectedHab;
};

#endif //_BOTTOMBOX_INCLUDE
