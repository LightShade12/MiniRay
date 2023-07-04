#include "glutils.h"
#include "creator/stb/stb_image.h"

GLenum bytesPerPixel(GLenum format)
{
	if (format == GL_RGBA32F) return GL_FLOAT;
	else if (format == GL_RGBA) return GL_UNSIGNED_BYTE;
}

void GLbytestotexture(GLuint& texref, const void* data, int width, int height, GLenum format) {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &texref);
	glBindTexture(GL_TEXTURE_2D, texref);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGB, bytesPerPixel(format), data);

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