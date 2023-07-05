#pragma once
#include <memory>
#include "application/core/intern/layer/layer.h"
#include "application/core/intern/image/image.h"
#include "imgui/imgui.h"

class ViewportLayer : public Layer
{
public:

	virtual void OnAttach() override
	{
		m_image = std::make_shared<Image>("../test/textures/try.png");
	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("Viewport");
		if (m_image)
			ImGui::Image((void*)m_image->GetGLTexID(), ImVec2(m_image->GetWidth(), m_image->GetHeight()));
		ImGui::End();
	}
	GLuint m_testimageID;
	std::shared_ptr<Image> m_image = nullptr;
};