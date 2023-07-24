#include "attributemanager.h"

void drawAttributeManager(EditorLayer* editorlayer)
{
	ImGui::Begin("Attribute Manager");
	if (ImGui::CollapsingHeader("Object Properties", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (editorlayer->selection_mask >= 0) {
			//TODO: figure out the mechanism and fix
			int selection_index = log2(editorlayer->selection_mask);//nasty fix
			MeshModel& model = editorlayer->m_Scene.Models[selection_index];
			ImGui::InputTextWithHint("Name", model.name.c_str(), model.name.data(), 128);
			//ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1f);
			//ImGui::DragFloat("Radius", &sphere.Radius, 0.1f);
			if (ImGui::InputInt("Material Index", &model.MaterialIndex, 1, 1)) {
				if (model.MaterialIndex > (int)editorlayer->m_Scene.Materials.size() - 1) { model.MaterialIndex--; }
				else if (model.MaterialIndex < 0) { model.MaterialIndex++; }
			};
			ImGui::Text("Applied material: %s", editorlayer->m_Scene.Materials[model.MaterialIndex].name.c_str());

			ImGui::Separator();
		}
	}
	ImGui::End();
}