#pragma once
#include "glad/glad.h"

GLuint gentex(GLuint& tex);
void GLbytestotexture(GLuint& texref, const void* data, int width, int height, GLenum format);

GLenum bytesPerChannel(GLenum format);

class framebuffer {
public:
	unsigned int FBO, framebuffertexture;

	framebuffer(int width, int height);

	~framebuffer();

};

class screenVAOVBO {
public:

	unsigned int VAO, VBO;

	screenVAOVBO();

	~screenVAOVBO();
};
