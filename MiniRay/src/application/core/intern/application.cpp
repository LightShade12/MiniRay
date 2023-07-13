#include "application/core/application.h"
#include "AppUtils/AppUtils.h"
#include "UtilsCommon/UtilsCommon.h"
#include "imgui/imguithemes.h"
#include <glm/glm.hpp>

extern bool g_ApplicationRunning;

static application* s_Instance = nullptr;

application::application(const application_specification& app_spec) :m_specification(app_spec)
{
	s_Instance = this;
	init();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontDefault();
	mainfont = io.Fonts->AddFontFromFileTTF("resources//fonts//JetBrainsMono-Regular.ttf", 17.5f);
	ImGuithemes::dark();
	glClearColor(0.149, 0.275, 0.388, 1.0);
}

application::~application()
{
	shutdown();
	s_Instance = nullptr;
}

application& application::Get()
{
	return *s_Instance;
}

float application::GetTime()
{
	return (float)glfwGetTime();
}

#include <iostream>

void application::run()
{
	m_running = true;
	while (!glfwWindowShouldClose(m_WindowHandle) && m_running) {
		glClear(GL_COLOR_BUFFER_BIT);
		utils::getwindowsize(m_WindowHandle, &m_specification.width, &m_specification.height);
		for (auto& layer : m_LayerStack)
			layer->OnUpdate(m_TimeStep);

		AppImguiFrameStart();
		ImGui::PushFont(mainfont);
		//layer
		for (auto& layer : m_LayerStack)
			layer->OnUIRender();

		ImGui::PopFont();
		AppImguiFrameEnd();

		float time = GetTime();
		m_FrameTime = time - m_LastFrameTime;
		m_TimeStep = glm::min<float>(m_FrameTime, 0.0333f);
		m_LastFrameTime = time;

		glfwPollEvents();
		glfwSwapBuffers(m_WindowHandle);
	}
	if (glfwWindowShouldClose(m_WindowHandle)) {
		g_ApplicationRunning = false;
		application::Get().close();
	}
}

void application::close()
{
	m_running = false;//run depends on this; use to restart
}
void application::init()
{
	//do pre setup
	AppInit(m_WindowHandle, m_specification.name.c_str(), m_specification.iconpath.c_str(), m_specification.width, m_specification.height);
}
//dont use manually
void application::shutdown()
{
	for (auto& layer : m_LayerStack)
		layer->OnDetach();

	m_LayerStack.clear();
	AppTerminate(m_WindowHandle);
	//g_ApplicationRunning = false;
}
;