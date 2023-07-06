#pragma once
#include <memory>
#include "application/core/intern/layer/layer.h"
#include "application/core/intern/image/image.h"
#include "application/core/intern/timer/timer.h"
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
		ImGui::Text("last render time: %.3fms", m_lastrendertime);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		ImGui::Begin("Viewport");
		m_viewportWidth = ImGui::GetContentRegionAvail().x;
		m_viewportHeight = ImGui::GetContentRegionAvail().y;
		if (m_image)
			ImGui::Image((void*)m_image->GetGLTexID(), ImVec2(m_image->GetWidth(), m_image->GetHeight()));
		ImGui::End();

		ImGui::PopStyleVar();

		Render();
	}

	void Render() {
		Timer timer;

		if (!m_image || m_viewportWidth != m_image->GetWidth() || m_viewportHeight != m_image->GetHeight())
		{
			m_image = std::make_shared<Image>(m_viewportWidth, m_viewportHeight, GL_RGB);
			delete[] m_data;
			m_data = new GLubyte[m_viewportWidth * m_viewportHeight * 3];
		}

		for (int i = 0; i < m_viewportHeight * m_viewportWidth * 3; i = i + 3) {
			m_data[i] = 255;//r
			m_data[i + 1] = 0;//g
			m_data[i + 2] = 255;//b
		}

		m_image->uploadDatatoGPU(m_data);

		m_lastrendertime = timer.ElapsedMillis();
	};
	float m_lastrendertime = 0;
	GLubyte* m_data = nullptr;
	uint32_t m_viewportWidth = 0, m_viewportHeight = 0;
	GLuint m_testimageID;
	std::shared_ptr<Image> m_image = nullptr;
};