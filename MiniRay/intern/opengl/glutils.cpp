#include "glutils.h"
#include "stb/stb_image.h"

GLfloat rectangleVertices[24] =
{
	// Coords    // texCoords
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,

	 1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
};

GLenum bytesPerChannel(GLenum format)
{
	if (format == GL_RGBA32F) return GL_FLOAT;
	else if (format == GL_RGB32F) return GL_FLOAT;
	else if (format == GL_RGBA) return GL_UNSIGNED_BYTE;
	else if (format == GL_RGB) return GL_UNSIGNED_BYTE;
	else throw("Non supported image buffer format");
}

GLenum outputBufferFormat(GLenum internalformat)
{
	if (internalformat == GL_RGBA32F) return GL_RGBA;
	else if (internalformat == GL_RGB32F) return GL_RGB;
	else if (internalformat == GL_RGBA) return GL_RGBA;
	else if (internalformat == GL_RGB) return GL_RGB;
	else throw("Non supported image buffer format");
}

void GLbytestotexture(GLuint& texref, const void* data, int width, int height, GLenum format) {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &texref);
	glBindTexture(GL_TEXTURE_2D, texref);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGB, bytesPerChannel(format), data);

	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint gentex(GLuint& tex) {
	int imgwidth, imgheight, imgnumcolch;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* imgbytes = stbi_load("resources\\textures\\try.png", &imgwidth, &imgheight, &imgnumcolch, 0);

	glGenTextures(1, &tex);

	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgwidth, imgheight, 0, GL_RGB, GL_UNSIGNED_BYTE, imgbytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(imgbytes);
	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;
}

framebuffer::framebuffer(int width, int height) {
	// Create Frame Buffer Object
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Create Framebuffer Texture
	glGenTextures(1, &framebuffertexture);
	glBindTexture(GL_TEXTURE_2D, framebuffertexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffertexture, 0);
};

framebuffer::~framebuffer() {
	glDeleteTextures(1, &framebuffertexture);
	glDeleteFramebuffers(1, &FBO);
};

screenVAOVBO::screenVAOVBO() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);//check if sizeof() returns the right value i.e "arraysize"
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
};

screenVAOVBO::~screenVAOVBO() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
};