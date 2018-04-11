#ifndef _BUTTON_INCLUDE
#define _BUTTON_INCLUDE

#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include <functional>

class Button {

public:
	// Quads can only be created inside an OpenGL context
	static Button *createButton(glm::vec2 geom[2], glm::vec2 texCoords[2], std::function<void()> callback, Texture *buttonTex, ShaderProgram *program);

	Button(glm::vec2 geom[2], glm::vec2 texCoords[2], std::function<void()> callback, Texture *buttonTex, ShaderProgram *program);

	void render() const;
	void free();
	bool isMouseOver();

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

private:
	Texture *texture;
	ShaderProgram *shaderProgram;
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	std::function<void()> callback;
	float x, y, width, height;
	bool mouseOver;

};

#endif //_BUTTON_INCLUDE