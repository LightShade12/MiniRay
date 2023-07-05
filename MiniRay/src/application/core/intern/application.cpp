#include "application/core/application.h"
#include "AppUtils/AppUtils.h"
#include "UtilsCommon/UtilsCommon.h"
#include "imgui/imguithemes.h"

extern bool g_ApplicationRunning;

application::application(const application_specification& app_spec) :m_specification(app_spec)
{
	init();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontDefault();
	mainfont = io.Fonts->AddFontFromFileTTF("resources\\fonts\\JetBrainsMono-Regular.ttf", 17.5f);
	ImGuithemes::UE4();
	glClearColor(0.149, 0.275, 0.388, 1.0);
}

application::~application()
{
	shutdown();
}

void application::run()
{
	m_running = true;
	while (!glfwWindowShouldClose(WindowHandle) && m_running) {
		glClear(GL_COLOR_BUFFER_BIT);
		utils::getwindowsize(WindowHandle, &m_specification.width, &m_specification.height);
		//for (auto& layer : m_LayerStack)
		//	layer->OnUpdate(m_TimeStep);

		AppImguiFrameStart();
		ImGui::PushFont(mainfont);
		//layer
		for (auto& layer : m_LayerStack)
			layer->OnUIRender();

		ImGui::PopFont();
		AppImguiFrameEnd();
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
	for (auto& layer : m_LayerStack)
		layer->OnDetach();

	m_LayerStack.clear();
	AppTerminate(WindowHandle);
	g_ApplicationRunning = false;
}
;