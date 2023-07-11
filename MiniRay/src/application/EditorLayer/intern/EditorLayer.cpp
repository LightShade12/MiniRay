#include "../EditorLayer.h"

EditorLayer::EditorLayer()
	:m_camera(45, 01, 100)
{
	Material& pinkSphere = m_Scene.Materials.emplace_back();
	pinkSphere.Albedo = { 1.0f, 0.0f, 1.0f };
	pinkSphere.Roughness = 0.0f;

	Material& blueSphere = m_Scene.Materials.emplace_back();
	blueSphere.Albedo = { 0.2f, 0.3f, 1.0f };
	blueSphere.Roughness = 0.1f;

	Material& orangeSphere = m_Scene.Materials.emplace_back();
	orangeSphere.Albedo = { 0.8f, 0.5f, 0.2f };
	orangeSphere.Roughness = 0.1f;
	orangeSphere.EmissionColor = orangeSphere.Albedo;
	orangeSphere.EmissionPower = 2.0f;

	{
		Sphere sphere;
		sphere.Position = { 0.0f, 0.0f, 0.0f };
		sphere.Radius = 1.0f;
		sphere.MaterialIndex = 0;
		m_Scene.Spheres.push_back(sphere);
	}
	{
		Sphere sphere;
		sphere.Position = { 0.0f, 0.0f, 0.0f };
		sphere.Radius = 1.0f;
		sphere.MaterialIndex = 0;
		m_Scene.Spheres.push_back(sphere);
	}
	{
		Sphere sphere;
		sphere.Position = { 0.0f, 0.0f, 0.0f };
		sphere.Radius = 1.0f;
		sphere.MaterialIndex = 0;
		m_Scene.Spheres.push_back(sphere);
	}
	//emission sphere
	{
		Sphere sphere;
		sphere.Position = { 2.0f, 0.0f, 0.0f };
		sphere.Radius = 1.0f;
		sphere.MaterialIndex = 2;
		m_Scene.Spheres.push_back(sphere);
	}
	//ground sphere
	{
		Sphere sphere;
		sphere.Position = { 0.0f, -1001.0f, 0.0f };
		sphere.Radius = 1000.0f;
		sphere.MaterialIndex = 1;
		m_Scene.Spheres.push_back(sphere);
	}
};

void EditorLayer::OnUIRender()
{
	ImGui::BeginMainMenuBar();
	ImGui::MenuItem("File");
	ImGui::MenuItem("Edit");
	ImGui::MenuItem("View");
	ImGui::MenuItem("Window");
	ImGui::MenuItem("Help");
	ImGui::InputTextWithHint("", "search actions", m_top_str_buffer, IM_ARRAYSIZE(m_str_buffer));
	ImGui::EndMainMenuBar();

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), m_dockflags);
	if (ImGui::BeginViewportSideBar("##MainStatusBar", NULL, ImGuiDir_Down, ImGui::GetFrameHeight(), m_windowflags)) {
		if (ImGui::BeginMenuBar()) {
			ImGui::Text("Status");
			ImGui::EndMenuBar();
		}
	};

	ImGui::Begin("settings");

	for (size_t i = 0; i < m_Scene.Materials.size(); i++)
	{
		ImGui::PushID(i);

		Material& material = m_Scene.Materials[i];
		ImGui::ColorEdit3("Albedo", glm::value_ptr(material.Albedo));
		ImGui::DragFloat("Roughness", &material.Roughness, 0.05f, 0.0f, 1.0f);
		ImGui::DragFloat("Metallic", &material.Metallic, 0.05f, 0.0f, 1.0f);
		ImGui::ColorEdit3("Emission Color", glm::value_ptr(material.EmissionColor));
		ImGui::DragFloat("Emission Power", &material.EmissionPower, 0.05f, 0.0f, FLT_MAX);

		ImGui::Separator();

		ImGui::PopID();
	}

	ImGui::End();

	//------------------------------------------------------------------------------------------------------------------------------
	ImGui::Begin("Scene Graph");
	ImGui::InputTextWithHint("", "start typing to search", m_str_buffer, IM_ARRAYSIZE(m_str_buffer));

	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;
	ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

	// 'node_clicked' is temporary storage of what node we have clicked to process selection at the end
	/// of the loop. May be a pointer to your own node type, etc.
	static int selection_mask = (1 << 2);
	//static int selection_mask = -1;
	int node_clicked = -1;
	Sphere* selected_object = nullptr;

	for (int i = 0; i < m_Scene.Spheres.size(); i++)
	{
		// Disable the default "open on single-click behavior" + set Selected flag according to our selection.
		// To alter selection we use IsItemClicked() && !IsItemToggledOpen(), so clicking on an arrow doesn't alter selection.
		ImGuiTreeNodeFlags node_flags = base_flags;
		const bool is_selected = (selection_mask & (1 << i)) != 0;
		if (is_selected)
			node_flags |= ImGuiTreeNodeFlags_Selected;

		auto sphere = m_Scene.Spheres[i];
		{
			// Items 3..5 are Tree Leaves
			// The only reason we use TreeNode at all is to allow selection of the leaf. Otherwise we can
			// use BulletText() or advance the cursor by GetTreeNodeToLabelSpacing() and call Text().
			node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; //| ImGuiTreeNodeFlags_Bullet;
			ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, std::string(sphere.name + "%d").c_str(), i);
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
				node_clicked = i;
		}
	}
	if (node_clicked != -1)
	{
		// Update selection state
		// (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
		selection_mask = (1 << node_clicked);             // Click to single-select
	}
	ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
	ImGui::TreePop();

	ImGui::End();
	//------------------------------------------------------------------------------------------------------------------------------

	ImGui::Begin("Attribute Manager");
	if (ImGui::CollapsingHeader("Object Properties"))
	{
		if (selection_mask >= 0) {
			//TODO: look into and fix the mechanism
			auto selection_index = (selection_mask > m_Scene.Spheres.size()) ? selection_mask - 2 : selection_mask - 1;//nasty fix
			std::cerr << selection_index << "\n";
			Sphere& sphere = m_Scene.Spheres[selection_index];
			//ImGui::InputText("Name", );
			ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1f);
			ImGui::DragFloat("Radius", &sphere.Radius, 0.1f);
			ImGui::DragInt("Material", &sphere.MaterialIndex, 1.0f, 0, (int)m_Scene.Materials.size() - 1);

			ImGui::Separator();
		}
	}
	ImGui::End();

	ImGui::Begin("Inspector");
	ImGui::Text("Last render time: %.3fms", m_lastrendertime);
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
				switch (item_current_idx)
				{
				case 0:

					if (ImGui::Button("Reset buffer"))
						m_Renderer.ResetFrameIndex();
					ImGui::InputInt("Ray bounces", &m_Renderer.GetSettings().Bounces);
					ImGui::Checkbox("Acumulation", &m_Renderer.GetSettings().Accumulate);
					ImGui::Checkbox("mt1997 RNG", &m_Renderer.GetSettings().mt1997_Random);

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

	ImGui::Begin("Editor");
	//ImGui::Button("Button");
	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("Viewport");

	m_viewportWidth = ImGui::GetContentRegionAvail().x;
	m_viewportHeight = ImGui::GetContentRegionAvail().y;

	auto image = m_Renderer.GetFinalImage();
	if (image)
		ImGui::Image((void*)image->GetGLTexID(), ImVec2(image->GetWidth(), image->GetHeight()), { 0,1 }, { 1,0 });
	ImGui::End();

	ImGui::PopStyleVar();

	Render();
}