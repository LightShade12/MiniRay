#include "scenegraph.h"
#include "imgui/imgui_ext.h"

void drawSceneGraph(EditorLayer* editorlayer)
{
	ImGui::Begin("Scene Graph");
	ImGui::InputTextWithHint("###text2", "start typing to search", editorlayer->m_str_buffer, IM_ARRAYSIZE(editorlayer->m_str_buffer));

	//ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

	// 'node_clicked' is temporary storage of what node we have clicked to process selection at the end
	/// of the loop. May be a pointer to your own node type, etc.
	//static int selection_mask = (1 << 2);
	//static int selection_mask = -1;
	int node_clicked = -1;
	//Sphere* selected_object = nullptr;

	DrawRowsBackground(editorlayer->m_Scene.Models.size() + 5, ImGui::GetTextLineHeight() + editorlayer->item_spacing_y, ImGui::GetCurrentWindow()->WorkRect.Min.x, ImGui::GetCurrentWindow()->WorkRect.Max.x, editorlayer->item_offset_y, 0, ImGui::GetColorU32(ImVec4(0.2f, 0.2f, 0.2f, 0.4f)));

	std::vector<std::string>objnames(editorlayer->m_Scene.Models.size(), "name not found");

	for (int i = 0; i < editorlayer->m_Scene.Models.size(); i++)
	{
		// Disable the default "open on single-click behavior" + set Selected flag according to our selection.
		// To alter selection we use IsItemClicked() && !IsItemToggledOpen(), so clicking on an arrow doesn't alter selection.
		static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;
		ImGuiTreeNodeFlags node_flags = editorlayer->m_base_flags;
		const bool is_selected = (editorlayer->selection_mask & (1 << i)) != 0;
		if (is_selected)
			node_flags |= ImGuiTreeNodeFlags_Selected;
		MeshModel model = editorlayer->m_Scene.Models[i];
		bool duplicatename = std::find(objnames.begin(), objnames.end(), model.name) != objnames.end();
		{
			// Items 3..5 are Tree Leaves
			// The only reason we use TreeNode at all is to allow selection of the leaf. Otherwise we can
			// use BulletText() or advance the cursor by GetTreeNodeToLabelSpacing() and call Text().
			node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; //| ImGuiTreeNodeFlags_Bullet;
			if (model.name.find(editorlayer->m_str_buffer) != std::string::npos)
				//ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, std::string(sphere.name + "%d").c_str(), i + 1);
				ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags,
					(duplicatename) ?
					std::string(model.name + std::to_string(i)).c_str() : model.name.c_str());
			if (!duplicatename)objnames[i] = model.name;

			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				node_clicked = i;
			}
		}
	}

	if (node_clicked != -1)
	{
		// Update selection state
		// (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
		editorlayer->selection_mask = (1 << node_clicked);             // Click to single-select
	}
	//ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
	ImGui::TreePop();
	ImGui::End();
}