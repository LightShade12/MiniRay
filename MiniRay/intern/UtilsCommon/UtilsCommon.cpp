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