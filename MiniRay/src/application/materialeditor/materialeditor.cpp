#include "materialeditor.h"
#include "imgui/imgui_ext.h"

void drawMaterialEditor(EditorLayer* editorlayer)
{
	static int mat_selection_mask = (1);
	//static int selection_mask = -1;
	int mat_node_clicked = -1;
	Material* mat_selected_object = nullptr;

	ImGui::Begin("Editor");
	ImGui::BeginChild("matlist", { ImGui::GetContentRegionAvail().x / 5,ImGui::GetContentRegionAvail().y }, true);
	ImGui::Text("Materials List");
	ImGui::PushItemWidth(150);
	ImGui::InputTextWithHint("###text3", "Search materials", editorlayer->m_mat_str_buffer, IM_ARRAYSIZE(editorlayer->m_mat_str_buffer));
	ImGui::PopItemWidth();
	DrawRowsBackground(editorlayer->m_Scene.Materials.size() + 5, ImGui::GetTextLineHeight() + editorlayer->item_spacing_y, ImGui::GetCurrentWindow()->WorkRect.Min.x, ImGui::GetCurrentWindow()->WorkRect.Max.x, editorlayer->item_offset_y, 0, ImGui::GetColorU32(ImVec4(0.2f, 0.2f, 0.2f, 0.4f)));
	ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

	for (int j = 0; j < editorlayer->m_Scene.Materials.size(); j++)
	{
		// Disable the default "open on single-click behavior" + set Selected flag according to our selection.
		// To alter selection we use IsItemClicked() && !IsItemToggledOpen(), so clicking on an arrow doesn't alter selection.
		ImGuiTreeNodeFlags mat_node_flags = editorlayer->m_base_flags;
		const bool mat_is_selected = (mat_selection_mask & (1 << j)) != 0;
		if (mat_is_selected)
			mat_node_flags |= ImGuiTreeNodeFlags_Selected;

		auto material = editorlayer->m_Scene.Materials[j];

		{
			// Items 3..5 are Tree Leaves
			// The only reason we use TreeNode at all is to allow selection of the leaf. Otherwise we can
			// use BulletText() or advance the cursor by GetTreeNodeToLabelSpacing() and call Text().
			mat_node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; //| ImGuiTreeNodeFlags_Bullet;
			if (material.name.find(editorlayer->m_mat_str_buffer) != std::string::npos)
				ImGui::TreeNodeEx((void*)(intptr_t)j, mat_node_flags, material.name.c_str());
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				mat_node_clicked = j;
			}
		}
	}
	if (mat_node_clicked != -1)
	{
		// Update selection state
		// (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
		mat_selection_mask = (1 << mat_node_clicked);             // Click to single-select
	}
	ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());

	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("matedit");

	ImGui::Text("Material Editor");
	ImGui::BeginChild("matpreview", { 128,128 }, true);
	ImGui::Text("preview placeholder");
	ImGui::EndChild();
	if (mat_selection_mask >= 0) {
		//TODO: figure out the mechanism and fix
		int mat_selection_index = log2(mat_selection_mask);//nasty fix
		//std::cerr << selection_index<< "\n";
		Material& material = editorlayer->m_Scene.Materials[mat_selection_index];
		ImGui::Text(material.name.c_str());
		ImGui::Text("Material index:%d", mat_selection_index);
		ImGui::ColorEdit3("Albedo", glm::value_ptr(material.Albedo));
		ImGui::DragFloat("Roughness", &material.Roughness, 0.05f, 0.0f, 1.0f);
		ImGui::DragFloat("Metallic", &material.Metallic, 0.05f, 0.0f, 1.0f);
		ImGui::ColorEdit3("Emission Color", glm::value_ptr(material.EmissionColor));
		ImGui::DragFloat("Emission Power", &material.EmissionPower, 0.05f, 0.0f, FLT_MAX);
	}
	ImGui::EndChild();
	ImGui::End();
}