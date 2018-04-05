#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Button.h"


Button *Button::createButton(glm::vec2 geom[2], glm::vec2 texCoords[2], std::function<void()> callback, Texture *buttonTex, ShaderProgram *program)
{
	Button *quad = new Button(geom, texCoords, callback, buttonTex, program);

	return quad;
}


Button::Button(glm::vec2 geom[2], glm::vec2 texCoords[2], std::function<void()> callback, Texture *buttonTex, ShaderProgram *program)
{
	this->callback = callback;
	this->x = geom[0].x;
	this->y = geom[0].y;
	this->width = geom[1].x - geom[0].x;
	this->height = geom[1].y - geom[0].y;
	float vertices[24] = { geom[0].x, geom[0].y, texCoords[0].x, texCoords[0].y,
		geom[1].x, geom[0].y, texCoords[1].x, texCoords[0].y,
		geom[1].x, geom[1].y, texCoords[1].x, texCoords[1].y,
		geom[0].x, geom[0].y, texCoords[0].x, texCoords[0].y,
		geom[1].x, geom[1].y, texCoords[1].x, texCoords[1].y,
		geom[0].x, geom[1].y, texCoords[0].x, texCoords[1].y };
	this->texture = buttonTex;
	this->shaderProgram = program;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocation = program->bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program->bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
}

void Button::render() const
{
	glm::mat4 modelview = glm::mat4(1.0f);
	glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	shaderProgram->setTextureUnit("tex", 0);
	glActiveTexture(GL_TEXTURE0);
	this->texture->use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

void Button::free()
{
	glDeleteBuffers(1, &vbo);
}

void Button::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	if ((bLeftButton || bRightButton) && (x <= mouseX/3) && (mouseX/3 <= (x + width)) && (y <= mouseY/3) && (mouseY/3 <= (y + height)))
		callback(); // En caso de que se haga click izquierdo o derecho, llamamos al callback
}