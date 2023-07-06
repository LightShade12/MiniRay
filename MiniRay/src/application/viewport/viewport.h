#pragma once
#include <memory>
#include "application/core/intern/layer/layer.h"
#include "application/core/intern/image/image.h"
#include "application/core/intern/timer/timer.h"
#include "application/renderer/renderer.h"
#include "imgui/imgui.h"
#include "glm/glm.hpp"

class ViewportLayer : public Layer
{
public:

	virtual void OnAttach() override
	{
		m_renderer = std::make_shared<renderer>();
		//m_image = std::make_shared<Image>("../test/textures/try.png");
	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("settings");
		ImGui::Text("last render time: %.3fms", m_lastrendertime);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		ImGui::Begin("Viewport");
		m_viewportWidth = ImGui::GetContentRegionAvail().x;
		m_viewportHeight = ImGui::GetContentRegionAvail().y;
		
		auto image = m_renderer->GetFinalImage();
		if (image)
			ImGui::Image((void*)image->GetGLTexID(), ImVec2(image->GetWidth(), image->GetHeight()));
		ImGui::End();

		ImGui::PopStyleVar();

		Render();
	}

	void Render() {
		Timer timer;
		m_renderer->OnResize(m_viewportWidth, m_viewportHeight);

		m_renderer->render();

		m_lastrendertime = timer.ElapsedMillis();
	};
	float m_lastrendertime = 0;
	std::shared_ptr<renderer> m_renderer=nullptr;
	uint32_t m_viewportWidth = 0, m_viewportHeight = 0;
};