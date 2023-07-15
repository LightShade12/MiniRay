#include "UtilsCommon/UtilsCommon.h"

float utils::RGB8tofloat(int RGBval) {
	float normalised = RGBval / 255.f;

	return normalised;
};

void utils::getwindowsize(GLFWwindow* win, uint32_t* xwide, uint32_t* ytall)
{
	int a = (int)xwide / 2;
	int b = (int)ytall / 2;
	glfwGetWindowPos(win, &a, &b);
	glfwGetWindowSize(win, (int*)xwide, (int*)ytall);
};

utils::timer::timer(int a) {
	m_starttime = std::chrono::high_resolution_clock::now();
};

void utils::timer::stop() {
	auto endtime = std::chrono::high_resolution_clock::now();

	auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_starttime).time_since_epoch().count();
	auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endtime).time_since_epoch().count();
	auto duration = end - start;
	msduration = duration * 0.001;
};

void utils::loadtexture(GLuint* texid, std::string_view imagepath)
{
	int imgwidth, imgheight, imgnumcolch;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* imgbytes = stbi_load(imagepath.data(), &imgwidth, &imgheight, &imgnumcolch, 0);

	glGenTextures(1, texid);

	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, *texid);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgwidth, imgheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgbytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(imgbytes);
	glBindTexture(GL_TEXTURE_2D, 0);
}