#pragma once
#include "creator/common.h"
#include <iostream>
#include <array>

class framebuffer {
public:
	unsigned int FBO, framebuffertexture;

	framebuffer(int width, int height);

	~framebuffer();

};

class screenVAOVBO {
public:

	unsigned int VAO, VBO;

	screenVAOVBO(GLfloat* vert, int arraysize);

	~screenVAOVBO();
};

//void glwriteimage(GLuint* texref, int width, int height, std::array <std::array<GLubyte[4], 1500>, 800>pixelstorage);