#include "sceneconfigurator.h"

void drawSceneConfigurator(EditorLayer* editorlayer) {
	ImGui::Begin("Scene Configuration");
	ImGui::Text("Last render time: %.3fms", editorlayer->m_lastrendertime);

	if (ImGui::Button((editorlayer->RenderEnabled) ? "Pause Renderer" : "Enable Renderer"))  editorlayer->RenderEnabled = !editorlayer->RenderEnabled;

	if (ImGui::BeginTabBar("renderBar", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Render settings"))
		{
			if (ImGui::CollapsingHeader("Kernel", ImGuiTreeNodeFlags_DefaultOpen))
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
				switch (item_current_idx)
				{
				case 0:

					if (ImGui::Button("Reset buffer"))
						editorlayer->m_Renderer.ResetFrameIndex();
					if (ImGui::InputInt("Ray bounces", &(editorlayer->m_Renderer.GetSettings().Bounces)))editorlayer->m_Renderer.ResetFrameIndex();
					if (ImGui::InputInt("Maximum Samples", &(editorlayer->m_Renderer.GetSettings().MaxSamplesLimit))) editorlayer->m_Renderer.ResetFrameIndex();
					if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNone))
						ImGui::SetTooltip("Set to 0 or negative to disable sample limit; Do not set to 1");
					if (ImGui::Checkbox("Acumulation", &(editorlayer->m_Renderer.GetSettings().Accumulate)))editorlayer->m_Renderer.ResetFrameIndex();
					ImGui::Checkbox("mt1997 RNG", &(editorlayer->m_Renderer.GetSettings().mt1997_Random));

					break;
				default:
					break;
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