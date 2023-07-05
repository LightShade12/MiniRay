#pragma once
#include "application/core/intern/layer/layer.h"
#include "application/core/intern/image/image.h"
#include "imgui/imgui.h"

class NodeInspectorLayer : public Layer
{
public:

	virtual void OnUIRender() override
	{
		ImGui::Begin("Inspector");
		ImGui::Button("Button");
		if (ImGui::BeginTabBar("MyTabBar", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Render settings"))
			{
				if (ImGui::CollapsingHeader("Kernel"))
				{
					const char* items[] = { "CPU PathTrace", "Info Channel", "OPENGL Preview", "GPU PathTrace", "Dev" };
					static int item_current_idx = 0; // Here we store our selection data as an index.
					const char* combo_preview_value = items[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)
					if (ImGui::BeginCombo("Renderer", combo_preview_value, ImGuiComboFlags_None))
					{
						for (int n = 0; n < IM_ARRAYSIZE(items); n++)
						{
							const bool is_selected = (item_current_idx == n);
							if (ImGui::Selectable(items[n], is_selected))
								item_current_idx = n;

							// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Scene"))
			{
				if (ImGui::CollapsingHeader("Camera"))
				{
				}
				if (ImGui::CollapsingHeader("Environment"))
				{
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Imaging settings"))
			{
				if (ImGui::CollapsingHeader("Post Processing"))
				{
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
};