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

			//TODO: currently one model has one mesh so mesh[0]; change this part in future
			if (ImGui::InputInt("Material Index", &model.m_Meshes[0].MaterialIndex, 1, 1)) {
				if (model.m_Meshes[0].MaterialIndex > (int)editorlayer->m_Scene.Materials.size() - 1) { model.m_Meshes[0].MaterialIndex--; }
				else if (model.m_Meshes[0].MaterialIndex < 0) { model.m_Meshes[0].MaterialIndex++; }
			};
			ImGui::Text("Applied material: %s", editorlayer->m_Scene.Materials[model.m_Meshes[0].MaterialIndex].name.c_str());

			ImGui::Separator();
		}
	}
	ImGui::End();
}