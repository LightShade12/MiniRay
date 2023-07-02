#include "application.h"
#include <iostream>
#include "creator/app.h"
#include "creator/utils/utils.h"

extern bool g_ApplicationRunning;

application::application(const application_specification& app_spec) :m_specification(app_spec)
{
	init();
}
application::~application()
{
	shutdown();
}

void application::run()
{
	m_running = true;
	std::cout << "running from: ";
	while (!glfwWindowShouldClose(WindowHandle) && m_running) {
		glClear(GL_COLOR_BUFFER_BIT);
		utils::getwindowsize(WindowHandle, &m_specification.width, &m_specification.height);

		glfwPollEvents();
		glfwSwapBuffers(WindowHandle);
	}
}

void application::close()
{
	m_running = false;//run depends on this
}
void application::init()
{
	//do pre setup
	AppInit(WindowHandle, m_specification.name.c_str(), m_specification.iconpath.c_str(), m_specification.width, m_specification.height);
}
void application::shutdown()
{
	AppTerminate(WindowHandle);
	g_ApplicationRunning = false;
}
;