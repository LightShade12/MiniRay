#pragma once
#include <memory>
#include "application/core/intern/layer/layer.h"
#include "application/core/application.h"
#include "application/core/intern/image/image.h"
#include "application/core/intern/timer/timer.h"
#include "application/renderer/renderer.h"
#include "application/camera/camera.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

class EditorLayer : public Layer
{
public:

	EditorLayer();

	virtual void OnUpdate(float ts) {
		if (m_camera.OnUpdate(ts))
			m_Renderer.ResetFrameIndex();
	}

	virtual void OnAttach() override
	{
	}

	virtual void OnUIRender() override;

	void Render() {
		if (RenderEnabled)
		{
			Timer timer;
			m_Renderer.OnResize(m_viewportWidth, m_viewportHeight);
			m_camera.OnResize(m_viewportWidth, m_viewportHeight);

			m_Renderer.render(m_Scene, m_camera);

			m_lastrendertime = timer.ElapsedMillis();
		}
	};
	ImGuiIO& io = ImGui::GetIO();
	bool RenderEnabled = true;
	Scene m_Scene;
	Camera m_camera;
	float m_lastrendertime = 0;
	renderer m_Renderer;
	uint32_t m_viewportWidth = 0, m_viewportHeight = 0;
	char m_str_buffer[128] = "";
	char m_mat_str_buffer[128] = "";
	char m_top_str_buffer[128] = "";
	ImGuiDockNodeFlags m_dockflags = ImGuiDockNodeFlags_PassthruCentralNode;
	ImGuiWindowFlags m_windowflags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
};