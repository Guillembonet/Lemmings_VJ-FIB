#ifndef _BUTTON_INCLUDE
#define _BUTTON_INCLUDE

#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include <functional>

class Button {

public:
	// Quads can only be created inside an OpenGL context
	static Button *createButton(glm::vec2 geom[2], glm::vec2 texCoords[2], ShaderProgram &program, std::function<void()> callback);

	Button(glm::vec2 geom[2], glm::vec2 texCoords[2], ShaderProgram &program, std::function<void()> callback);

	void render(ShaderProgram &program, const Texture &tex) const;
	void free();

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	std::function<void()> callback;
	float x, y, width, height;

};

#endif //_BUTTON_INCLUDE