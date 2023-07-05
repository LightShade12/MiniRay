#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "application/core/intern/layer/layer.h"
#include "imgui/imgui.h"

struct GLFWwindow;

struct application_specification {
	std::string name = "Default name";
	std::string iconpath = "resources/vgui/appicon.png";
	uint32_t width = 1600;
	uint32_t height = 900;
};

class application {
public:
	application(const application_specification& app_spec = application_specification());

	~application();

	void run();

	void close();

	template<typename T>
	void PushLayer()
	{
		static_assert(std::is_base_of<Layer, T>::value, "Pushed type is not subclass of Layer!");
		m_LayerStack.emplace_back(std::make_shared<T>())->OnAttach();
	}

	void PushLayer(const std::shared_ptr<Layer>& layer) { m_LayerStack.emplace_back(layer); layer->OnAttach(); }

private:
	void init();

	void shutdown();

	;
private:

	bool m_running = false;
	GLFWwindow* WindowHandle = nullptr;
	application_specification m_specification;

	//float m_TimeStep = 0.0f;
	//float m_FrameTime = 0.0f;
	//float m_LastFrameTime = 0.0f;
	ImFont* mainfont = nullptr;
	std::vector<std::shared_ptr<Layer>> m_LayerStack;
	std::function<void()> m_MenubarCallback;
};

application* create_application(int argc, char* argv[]);