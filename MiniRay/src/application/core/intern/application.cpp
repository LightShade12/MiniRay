#include "application/core/application.h"
#include "AppUtils/AppUtils.h"
#include "UtilsCommon/UtilsCommon.h"
#include "imgui/imguithemes.h"
#include <glm/glm.hpp>
#include "application/core/intern/hardware/CpuQuery.h"

extern bool g_ApplicationRunning;

static application* s_Instance = nullptr;

application::application(const application_specification& app_spec) :m_specification(app_spec)
{
	s_Instance = this;
	init();
	Get().m_HardwareInfo.cpuname = GetCpuInfo();

	utils::loadtexture(&guitexidlist[0], "resources/vgui/close.png");
	utils::loadtexture(&guitexidlist[1], "resources/vgui/maximize.png");
	utils::loadtexture(&guitexidlist[2], "resources/vgui/restore.png");
	utils::loadtexture(&guitexidlist[3], "resources/vgui/minimize.png");
	utils::loadtexture(&guitexidlist[4], "resources/vgui/appicon.png");

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontDefault();
	mainfont = io.Fonts->AddFontFromFileTTF("resources/fonts/JetBrainsMono-Regular.ttf", 17.5f);
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



void application::run()
{
	m_running = true;
	while (!glfwWindowShouldClose(m_WindowHandle) && m_running) {
		glClear(GL_COLOR_BUFFER_BIT);
		utils::getwindowsize(m_WindowHandle, &m_specification.width, &m_specification.height);
		glViewport(0, 0, m_specification.width, m_specification.height);
		for (auto& layer : m_LayerStack)
			layer->OnUpdate(m_TimeStep);

		AppImguiFrameStart();
		ImGui::PushFont(mainfont);
		ImGuiIO& io = ImGui::GetIO();

		if (ImGui::BeginViewportSideBar("##TopStatusBar", NULL, ImGuiDir_Up, ImGui::GetFrameHeight(), windowflags)) {
			if (ImGui::BeginMenuBar()) {
				ImGui::Image((void*)application::Get().guitexidlist[4], { 24,24 });
				ImGui::Text("MiniRay(prototype)");
				ImGui::InvisibleButton("handlebar", { ImGui::GetWindowWidth() - 270,100 });

				//Window Placment handling---------------------------------
				int count = IM_ARRAYSIZE(io.MouseDown);
				int mouse = -1;

				for (int i = 0; i < count; i++)
					if (ImGui::IsMouseDown(i))
					{
						mouse = i;
					}

				GetMonitorInfo(m_HMonitor, &m_HMonInfo);

				if (!(mouse == 0))HoveringOnTitlebar = ImGui::IsItemHovered();
				if (mouse == 0 && HoveringOnTitlebar)
				{
					if (m_Maximised)
					{
						glfwSetWindowSize(m_WindowHandle, 1600, 900);
						ImVec2 winmouserelativepos{ io.MousePos.x / m_HMonInfo.rcWork.right , io.MousePos.y / m_HMonInfo.rcWork.bottom };
						glfwSetWindowPos(m_WindowHandle, io.MousePos.x - winmouserelativepos.x * 1600, io.MousePos.y - winmouserelativepos.y * 900);//cursor needs to maintain relative pos with window
						m_Maximised = false;
					}
					else if (io.MousePos.y == 0) ShouldMaximiseWindow = true;

					glfwGetWindowPos(m_WindowHandle, &winxpos, &winypos);
					glfwSetWindowPos(m_WindowHandle, io.MouseDelta.x + winxpos, io.MouseDelta.y + winypos);
					if (winxpos == io.MousePos.x && winypos == io.MousePos.y)HoveringOnTitlebar = ImGui::IsItemHovered();
				}
				else if (ShouldMaximiseWindow)
				{
					glfwSetWindowPos(m_WindowHandle, 0, 0);
					glfwSetWindowSize(m_WindowHandle, m_HMonInfo.rcWork.right, m_HMonInfo.rcWork.bottom - 1);//slight paddinhg to prevent gamemode fullscreen when taskbar is hidden
					ShouldMaximiseWindow = false;
					m_Maximised = true;
				}

				ImGui::SetCursorScreenPos({ ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x - 20, ImGui::GetCursorScreenPos().y });
				//minimise button
				if (ImGui::ImageButton((void*)guitexidlist[0], { 16,16 })) {
					g_ApplicationRunning = false; close();
				};
				//-------------------------

				ImGui::SetCursorScreenPos({ ImGui::GetCursorScreenPos().x - 60, ImGui::GetCursorScreenPos().y });
				
				//maximise button
				if (ImGui::ImageButton((m_Maximised) ? (void*)guitexidlist[2] : (void*)guitexidlist[1], { 16,16 }))
				{
					application::Get().m_Maximised = !application::Get().m_Maximised;
					if (!application::Get().m_Maximised)
					{
						glfwSetWindowPos(application::Get().GetWindowHandle(), 100, 100);
						glfwSetWindowSize(application::Get().GetWindowHandle(), 1600, 900);
						//glfwRestoreWindow(application::Get().GetWindowHandle());
					}
					else
					{
						glfwSetWindowPos(application::Get().GetWindowHandle(), 0, 0);
						glfwSetWindowSize(application::Get().GetWindowHandle(), application::Get().m_HMonInfo.rcWork.right, application::Get().m_HMonInfo.rcWork.bottom - 1);
					}
				};
				//-----------------
				ImGui::SetCursorScreenPos({ ImGui::GetCursorScreenPos().x - 60, ImGui::GetCursorScreenPos().y });

				//close button
				if (ImGui::ImageButton((void*)guitexidlist[3], { 16,16 }))glfwIconifyWindow(m_WindowHandle);

				ImGui::EndMenuBar();
				//--------------------
			}
		};

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

	for (auto texid : guitexidlist) {
		glDeleteTextures(1, &texid);
	}

	m_LayerStack.clear();
	AppTerminate(m_WindowHandle);
}
;