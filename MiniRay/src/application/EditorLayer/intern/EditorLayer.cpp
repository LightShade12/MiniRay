#include "../EditorLayer.h"
#include "imgui/imgui_ext.h"
#include <algorithm>
#include "GLFW/glfw3.h"

extern bool g_ApplicationRunning;
EditorLayer::EditorLayer()
	:m_camera(45, 01, 100)
{
	Material& material1 = m_Scene.Materials.emplace_back();
	material1.Albedo = { 0.24f, 0.98f, 0.88f };
	material1.Roughness = 0.0f;
	material1.EmissionColor = material1.Albedo;
	material1.name = "blue emit";
	material1.EmissionPower = 2.0f;

	Material& material2 = m_Scene.Materials.emplace_back();
	material2.Albedo = { 0.0f, 1.0f, 0.0f };
	material2.Roughness = 0.1f;
	material2.EmissionColor = material2.Albedo;
	material2.name = "green emit";
	material2.EmissionPower = 2.0f;

	Material& material3 = m_Scene.Materials.emplace_back();
	material3.Albedo = { 1.f, 0.f, 0.f };
	material3.Roughness = 0.1f;
	material3.EmissionColor = material3.Albedo;
	material3.name = "red emit";
	material3.EmissionPower = 2.0f;

	Material& material4 = m_Scene.Materials.emplace_back();
	material4.Albedo = { 1.f, 1.f, 1.f };
	material4.Roughness = 0.1f;
	material4.name = "white mat";

	Material& material5 = m_Scene.Materials.emplace_back();
	material5.Albedo = { 1.f, 0.f, 0.f };
	material5.Roughness = 0.1f;
	material5.name = "ground mat";

	{
		Sphere sphere;
		sphere.Position = { 0.0f, 0.0f, -2.0f };
		sphere.Radius = 1.0f;
		sphere.MaterialIndex = 3;
		m_Scene.Spheres.push_back(sphere);
	}
	{
		Sphere sphere;
		sphere.Position = { -2.0f, 0.0f, 2.0f };
		sphere.Radius = 1.0f;
		sphere.MaterialIndex = 3;
		m_Scene.Spheres.push_back(sphere);
	}
	{
		Sphere sphere;
		sphere.Position = { 2.0f, 0.0f, 2.0f };
		sphere.Radius = 1.0f;
		sphere.MaterialIndex = 3;
		m_Scene.Spheres.push_back(sphere);
	}
	//subject sphere
	{
		Sphere sphere;
		sphere.Position = { 0.0f, 0.0f, 0.0f };
		sphere.Radius = 1.0f;
		sphere.MaterialIndex = 0;
		sphere.name = "emission ball";
		m_Scene.Spheres.push_back(sphere);
	}
	//ground sphere
	{
		Sphere sphere;
		sphere.Position = { 0.0f, -1001.0f, 0.0f };
		sphere.Radius = 1000.0f;
		sphere.MaterialIndex = 4;
		sphere.name = "ground ball";
		m_Scene.Spheres.push_back(sphere);
	}
};

void EditorLayer::OnUIRender()
{
	//ImGui::ShowDemoWindow();
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Restart Engine"))
		{
			application::Get().close();
		}

		if (ImGui::MenuItem("Exit", "Alt+F4"))
		{
			g_ApplicationRunning = false;
			application::Get().close();
		}
		ImGui::EndMenu();
	}

	ImGui::MenuItem("Edit");
	ImGui::MenuItem("View");
	ImGui::MenuItem("Render");
	ImGui::MenuItem("Window");
	ImGui::MenuItem("Help");

	//ImGui::ShowDemoWindow();

	int count = IM_ARRAYSIZE(io.MouseDown);
	int mouse = 0;//right click?

	for (int i = 0; i < count; i++)
		if (ImGui::IsMouseDown(i))
		{
			mouse = i;
		}

	ImGui::InputTextWithHint("", "search actions", m_top_str_buffer, IM_ARRAYSIZE(m_str_buffer));

	static int winxpos = 0, winypos = 0;
	static bool maximised = false;

	//if (mouse == 2 && ImGui::IsItemHovered())
	if (mouse == 2 && !maximised)
	{
		glfwGetWindowPos(application::Get().GetWindowHandle(), &winxpos, &winypos);
		glfwSetWindowPos(application::Get().GetWindowHandle(), io.MouseDelta.x + winxpos, io.MouseDelta.y + winypos);
	}

	ImGui::SetCursorScreenPos({ ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x - 20, ImGui::GetCursorScreenPos().y });
	if (ImGui::ImageButton((void*)application::Get().guitexidlist[0], { 16,16 })) {
		g_ApplicationRunning = false; application::Get().close();
	};


	ImGui::SetCursorScreenPos({ ImGui::GetCursorScreenPos().x - 60, ImGui::GetCursorScreenPos().y });
	if (ImGui::ImageButton((maximised) ? (void*)application::Get().guitexidlist[2] : (void*)application::Get().guitexidlist[1], { 16,16 }))
	{
		maximised = !maximised;
		if (!maximised)
		{
			glfwSetWindowPos(application::Get().GetWindowHandle(), 100, 100);
			glfwSetWindowSize(application::Get().GetWindowHandle(), 1600, 900);
		}
		else
		{
			glfwSetWindowPos(application::Get().GetWindowHandle(), 0, 0);
			glfwSetWindowSize(application::Get().GetWindowHandle(), 1919, 1079);
		}
	};

	ImGui::SetCursorScreenPos({ ImGui::GetCursorScreenPos().x - 60, ImGui::GetCursorScreenPos().y });
	if (ImGui::ImageButton((void*)application::Get().guitexidlist[3], { 16,16 }))glfwIconifyWindow(application::Get().GetWindowHandle());

	ImGui::EndMainMenuBar();

	if (ImGui::BeginViewportSideBar("##MainStatusBar", NULL, ImGuiDir_Down, ImGui::GetFrameHeight(), m_windowflags)) {
		if (ImGui::BeginMenuBar()) {
			ImGui::Text("Status");
			ImGui::EndMenuBar();
		}
	};
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), m_dockflags);

	//------------------------------------------------------------------------------------------------------------------------------
	ImGui::Begin("Scene Graph");
	ImGui::InputTextWithHint("", "start typing to search", m_str_buffer, IM_ARRAYSIZE(m_str_buffer));

	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;
	ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

	// 'node_clicked' is temporary storage of what node we have clicked to process selection at the end
	/// of the loop. May be a pointer to your own node type, etc.
	//static int selection_mask = (1 << 2);
	static int selection_mask = (1);
	//static int selection_mask = -1;
	int node_clicked = -1;
	Sphere* selected_object = nullptr;

	float item_spacing_y = ImGui::GetStyle().ItemSpacing.y;
	float item_offset_y = -item_spacing_y * 0.5f;
	DrawRowsBackground(m_Scene.Spheres.size() + 5, ImGui::GetTextLineHeight() + item_spacing_y, ImGui::GetCurrentWindow()->WorkRect.Min.x, ImGui::GetCurrentWindow()->WorkRect.Max.x, item_offset_y, 0, ImGui::GetColorU32(ImVec4(0.2f, 0.2f, 0.2f, 0.4f)));

	std::vector<std::string>objnames(m_Scene.Spheres.size(), "name not found");

	for (int i = 0; i < m_Scene.Spheres.size(); i++)
	{
		// Disable the default "open on single-click behavior" + set Selected flag according to our selection.
		// To alter selection we use IsItemClicked() && !IsItemToggledOpen(), so clicking on an arrow doesn't alter selection.
		ImGuiTreeNodeFlags node_flags = base_flags;
		const bool is_selected = (selection_mask & (1 << i)) != 0;
		if (is_selected)
			node_flags |= ImGuiTreeNodeFlags_Selected;
		auto sphere = m_Scene.Spheres[i];
		bool duplicatename = std::find(objnames.begin(), objnames.end(), sphere.name) != objnames.end();
		{
			// Items 3..5 are Tree Leaves
			// The only reason we use TreeNode at all is to allow selection of the leaf. Otherwise we can
			// use BulletText() or advance the cursor by GetTreeNodeToLabelSpacing() and call Text().
			node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; //| ImGuiTreeNodeFlags_Bullet;
			if (sphere.name.find(m_str_buffer) != std::string::npos)
				//ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, std::string(sphere.name + "%d").c_str(), i + 1);
				ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags,
					(duplicatename) ?
					std::string(sphere.name + std::to_string(i)).c_str() : sphere.name.c_str());
			if (!duplicatename)objnames[i] = sphere.name;

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
		selection_mask = (1 << node_clicked);             // Click to single-select
	}
	ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
	ImGui::TreePop();
	ImGui::End();

	//std::cerr << (selection_mask | (1 << var)) << "\n";
	//------------------------------------------------------------------------------------------------------------------------------

	ImGui::Begin("Attribute Manager");
	if (ImGui::CollapsingHeader("Object Properties", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (selection_mask >= 0) {
			//TODO: figure out the mechanism and fix
			int selection_index = log2(selection_mask);//nasty fix
			Sphere& sphere = m_Scene.Spheres[selection_index];
			ImGui::InputTextWithHint("Name", sphere.name.c_str(), sphere.name.data(), 128);
			ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1f);
			ImGui::DragFloat("Radius", &sphere.Radius, 0.1f);
			if (ImGui::InputInt("Material Index", &sphere.MaterialIndex, 1, 1)) {
				if (sphere.MaterialIndex > (int)m_Scene.Materials.size() - 1) { sphere.MaterialIndex--; }
				else if (sphere.MaterialIndex < 0) { sphere.MaterialIndex++; }
			};
			ImGui::Text("Applied material: %s", m_Scene.Materials[sphere.MaterialIndex].name.c_str());

			ImGui::Separator();
		}
	}
	ImGui::End();

	ImGui::Begin("Scene Configuration");
	ImGui::Text("Last render time: %.3fms", m_lastrendertime);

	if (ImGui::Button((RenderEnabled) ? "Pause Renderer" : "Enable Renderer"))  RenderEnabled = !RenderEnabled;

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
						m_Renderer.ResetFrameIndex();
					if (ImGui::InputInt("Ray bounces", &m_Renderer.GetSettings().Bounces))m_Renderer.ResetFrameIndex();
					if (ImGui::InputInt("Maximum Samples", &m_Renderer.GetSettings().MaxSamplesLimit)) m_Renderer.ResetFrameIndex();
					if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNone))
						ImGui::SetTooltip("Set to 0 or negative to disable sample limit; Do not set to 1");
					if (ImGui::Checkbox("Acumulation", &m_Renderer.GetSettings().Accumulate))m_Renderer.ResetFrameIndex();
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

	//-------------------------------------------------------------------------------------------------------------------
	//EDITOR
	static int mat_selection_mask = (1);
	//static int selection_mask = -1;
	int mat_node_clicked = -1;
	Material* mat_selected_object = nullptr;

	ImGui::Begin("Editor");
	ImGui::BeginChild("matlist", { ImGui::GetContentRegionAvail().x / 5,ImGui::GetContentRegionAvail().y }, true);
	ImGui::Text("Materials List");
	ImGui::PushItemWidth(165);
	ImGui::InputTextWithHint("", "type here to search", m_mat_str_buffer, IM_ARRAYSIZE(m_mat_str_buffer));
	ImGui::PopItemWidth();
	DrawRowsBackground(m_Scene.Materials.size() + 5, ImGui::GetTextLineHeight() + item_spacing_y, ImGui::GetCurrentWindow()->WorkRect.Min.x, ImGui::GetCurrentWindow()->WorkRect.Max.x, item_offset_y, 0, ImGui::GetColorU32(ImVec4(0.2f, 0.2f, 0.2f, 0.4f)));

	for (int j = 0; j < m_Scene.Materials.size(); j++)
	{
		// Disable the default "open on single-click behavior" + set Selected flag according to our selection.
		// To alter selection we use IsItemClicked() && !IsItemToggledOpen(), so clicking on an arrow doesn't alter selection.
		ImGuiTreeNodeFlags mat_node_flags = base_flags;
		const bool mat_is_selected = (mat_selection_mask & (1 << j)) != 0;
		if (mat_is_selected)
			mat_node_flags |= ImGuiTreeNodeFlags_Selected;

		auto material = m_Scene.Materials[j];

		{
			// Items 3..5 are Tree Leaves
			// The only reason we use TreeNode at all is to allow selection of the leaf. Otherwise we can
			// use BulletText() or advance the cursor by GetTreeNodeToLabelSpacing() and call Text().
			mat_node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; //| ImGuiTreeNodeFlags_Bullet;
			if (material.name.find(m_mat_str_buffer) != std::string::npos)
				ImGui::TreeNodeEx((void*)(intptr_t)j, mat_node_flags, std::string(material.name + "%d").c_str(), j + 1);
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
		Material& material = m_Scene.Materials[mat_selection_index];
		ImGui::Text(material.name.c_str());
		ImGui::ColorEdit3("Albedo", glm::value_ptr(material.Albedo));
		ImGui::DragFloat("Roughness", &material.Roughness, 0.05f, 0.0f, 1.0f);
		ImGui::DragFloat("Metallic", &material.Metallic, 0.05f, 0.0f, 1.0f);
		ImGui::ColorEdit3("Emission Color", glm::value_ptr(material.EmissionColor));
		ImGui::DragFloat("Emission Power", &material.EmissionPower, 0.05f, 0.0f, FLT_MAX);
	}
	ImGui::EndChild();
	ImGui::End();
	//-------------------------------------------------------------------------------------------------------------------
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	ImGui::Begin("Render viewport");
	m_viewportWidth = ImGui::GetContentRegionAvail().x;
	m_viewportHeight = ImGui::GetContentRegionAvail().y - ((ImGui::GetContentRegionAvail().y < 22) ? 0 : 22);

	auto image = m_Renderer.GetFinalImage();
	if (image)
		ImGui::Image((void*)image->GetGLTexID(), ImVec2(image->GetWidth(), image->GetHeight()), { 0,1 }, { 1,0 });

	//viewportbar-------------------------------------
	static float timeelapsed = 0;

	if (RenderEnabled)
		timeelapsed += m_lastrendertime;
	if (m_Renderer.GetSampleCount() == 1)timeelapsed = 0;
	ImGui::BeginChild("statusbar", ImVec2(ImGui::GetContentRegionAvail().x, 19.0f));

	ImGui::GetCurrentWindow()->DC.LayoutType = ImGuiLayoutType_Horizontal;

	ImGui::SetCursorScreenPos({ ImGui::GetCursorScreenPos().x + 5, ImGui::GetCursorScreenPos().y + 4 });
	ImGui::ProgressBar((float)m_Renderer.GetSampleCount() / m_Renderer.GetSettings().MaxSamplesLimit, ImVec2(100.f, 10.f), "");

	ImGui::SetCursorScreenPos({ ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y - 7 });
	ImGui::Text("%d/%d %s", m_Renderer.GetSampleCount(), m_Renderer.GetSettings().MaxSamplesLimit, "s/px,");

	ImGui::SetCursorScreenPos({ ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y - 7 });
	ImGui::Text("%.1fs", timeelapsed / 1000);

	ImGui::SetCursorScreenPos({ (ImGui::GetWindowPos().x + ImGui::GetWindowWidth()) - ImGui::CalcTextSize(application::Get().GetHardwareData().cpuname.c_str()).x - 10, ImGui::GetCursorScreenPos().y - 7 });
	ImGui::Text(application::Get().GetHardwareData().cpuname.c_str());

	ImGui::GetCurrentWindow()->DC.LayoutType = ImGuiLayoutType_Vertical;
	ImGui::EndChild();
	ImGui::End();

	ImGui::PopStyleVar();

	Render();
}