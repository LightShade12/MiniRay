#pragma once
#include <chrono>
#include "common.h"
#include "stb/stb_image.h"
#include "glad/glad.h"

namespace utils {
	float RGB8tofloat(int RGBval);

	//updates xwide and ytall as width and height values,REQUIRES INITIALISED WIDTH AND HEIGHT
	void getwindowsize(GLFWwindow* win, uint32_t* xwide, uint32_t* ytall);

	class timer {
	public:

		timer(int a);

		void stop();

		double msduration = 0;

	private:

		std::chrono::time_point<std::chrono::high_resolution_clock> m_starttime;
	};

	//loads texture to GPU
	void loadtexture(GLuint* texid, std::string_view imagepath);
};

//-----------------------------
