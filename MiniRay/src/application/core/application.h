#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "application/core/intern/layer/layer.h"
#include "imgui/imgui.h"
#include "glad/glad.h"
#include <Windows.h>
#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_NATIVE_INCLUDE_NONE
#include "GLFW/glfw3native.h"

struct GLFWwindow;

struct application_specification {
	std::string name = "Default name";
	std::string iconpath = "resources/vgui/appicon.png";
	uint32_t width = 1600;
	uint32_t height = 900;
};

class application {
private:

	struct hardwareinfo {
		std::string cpuname = "Not detected";
		std::string gpuname = "not detected";
	};
public:
	application(const application_specification& app_spec = application_specification());

	~application();

	void run();

	//returns app instance reference
	static application& Get();

	float GetTime();

	void close();

	GLFWwindow* GetWindowHandle() const { return m_WindowHandle; }

	template<typename T>
	void PushLayer()
	{
		static_assert(std::is_base_of<Layer, T>::value, "Pushed type is not subclass of Layer!");
		m_LayerStack.emplace_back(std::make_shared<T>())->OnAttach();
	}

	void PushLayer(const std::shared_ptr<Layer>& layer) { m_LayerStack.emplace_back(layer); layer->OnAttach(); }

	hardwareinfo GetHardwareData() const { return m_HardwareInfo; };

	GLuint guitexidlist[5];
	HMONITOR m_HMonitor = MonitorFromWindow(glfwGetWin32Window(m_WindowHandle), MONITOR_DEFAULTTONEAREST);//windows api handle
	MONITORINFO m_HMonInfo = { sizeof(m_HMonInfo) };
	bool m_Maximised = true;

private:
	hardwareinfo m_HardwareInfo;


	void init();

	void shutdown();

private:

	bool m_running = false;
	GLFWwindow* m_WindowHandle = nullptr;
	application_specification m_specification;
	ImGuiWindowFlags windowflags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;;

	int winxpos = 0, winypos = 0;
	bool ShouldMaximiseWindow = false;
	bool HoveringOnTitlebar = false;
	float m_TimeStep = 0.0f;
	float m_FrameTime = 0.0f;
	float m_LastFrameTime = 0.0f;
	ImFont* mainfont = nullptr;
	std::vector<std::shared_ptr<Layer>> m_LayerStack;
	std::function<void()> m_MenubarCallback;
};

application* create_application(int argc, char* argv[]);