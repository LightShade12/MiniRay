#pragma once
#include "glad/glad.h"

GLuint gentex(GLuint& tex);
void GLbytestotexture(GLuint& texref, const void* data, int width, int height, GLenum format);
