#pragma once
#include <memory>
#include "application/core/intern/layer/layer.h"
#include "application/core/intern/image/image.h"
#include "imgui/imgui.h"
#include "glm/glm.hpp"

class ViewportLayer : public Layer
{
public:

	virtual void OnAttach() override
	{
		//m_image = std::make_shared<Image>("../test/textures/try.png");
	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("settings");
		if (ImGui::Button("Render"))
			Render();

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		ImGui::Begin("Viewport");
		m_viewportWidth = ImGui::GetContentRegionAvail().x;
		m_viewportHeight = ImGui::GetContentRegionAvail().y;
		if (m_image)
			ImGui::Image((void*)m_image->GetGLTexID(), ImVec2(m_image->GetWidth(), m_image->GetHeight()));
		ImGui::End();
		ImGui::PopStyleVar();
	}

	void Render() {
		if (!m_image || m_viewportWidth != m_image->GetWidth() || m_viewportHeight != m_image->GetHeight())
		{
			m_image = std::make_shared<Image>(m_viewportWidth, m_viewportHeight, GL_RGB);
			delete[] m_data;
			m_data = new uint32_t[m_viewportWidth * m_viewportHeight];
		}

		for (int i = 0; i < m_viewportHeight * m_viewportWidth; i++) {
			m_data[i] = 0xffff00ff;
		}

		m_image->uploadDatatoGPU(m_data);
	};
	uint32_t* m_data = nullptr;
	uint32_t m_viewportWidth = 0, m_viewportHeight = 0;
	GLuint m_testimageID;
	std::shared_ptr<Image> m_image = nullptr;
};