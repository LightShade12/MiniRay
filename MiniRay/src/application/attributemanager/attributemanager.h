#pragma once
#include "application/core/intern/layer/layer.h"
#include "application/core/intern/image/image.h"
#include "imgui/imgui.h"

class AttributeManagerLayer : public Layer
{
public:

	virtual void OnUIRender() override
	{
		ImGui::Begin("Attribute Manager");
		if (ImGui::CollapsingHeader("Object Properties"))
		{
		}
		ImGui::End();
	}
};
