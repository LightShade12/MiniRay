#pragma once
#include <chrono>
#include <creator/common.h>

namespace utils {

	float RGB8tofloat(int RGBval);

	//updates xwide and ytall as width and height values,REQUIRES INITIALISED WIDTH AND HEIGHT
	void getwindowsize(GLFWwindow* win, uint32_t* xwide, uint32_t* ytall);

	class timer {
	public:

		timer(int a);


		void stop();

		double msduration=0;

	private:

		std::chrono::time_point<std::chrono::high_resolution_clock> m_starttime;

	};

	
};

//-----------------------------
