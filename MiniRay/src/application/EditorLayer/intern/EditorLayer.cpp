#include "../EditorLayer.h"
#include "imgui/imgui_ext.h"
#include <algorithm>
#include "GLFW/glfw3.h"

extern bool g_ApplicationRunning;
EditorLayer::EditorLayer()
	:m_camera(45, 01, 100)
{
	Material& material1 = m_Scene.Materials.emplace_back();
	material1.Albedo = { 0.8,0.8,0.8 };
	material1.Roughness = 0.0f;
	material1.EmissionColor = material1.Albedo;
	material1.name = "red mat";
	material1.EmissionPower = 0.0f;

	{
		MeshModel mesh1("../test/models/cube.glb");
		m_Scene.Models.push_back(mesh1);
		MeshModel plane("../test/models/plane.glb");
		m_Scene.Models.push_back(plane);
	}
	{
		//in CCW
		//std::vector<float>verts =
		//{
		//	// Vertex 0
		//	0.5f, 0.5f, 0.5f,   // (x, y, z) = (0.5, 0.5, 0.5)

		//	// Vertex 1
		//	0.5f, 0.5f, -0.5f,  // (x, y, z) = (0.5, 0.5, -0.5)

		//	// Vertex 2
		//	0.5f, -0.5f, -0.5f, // (x, y, z) = (0.5, -0.5, -0.5)

		//	// Vertex 3
		//	0.5f, -0.5f, 0.5f,  // (x, y, z) = (0.5, -0.5, 0.5)

		//	// Vertex 4
		//	-0.5f, 0.5f, 0.5f,  // (x, y, z) = (-0.5, 0.5, 0.5)

		//	// Vertex 5
		//	-0.5f, 0.5f, -0.5f, // (x, y, z) = (-0.5, 0.5, -0.5)

		//	// Vertex 6
		//	-0.5f, -0.5f, -0.5f,// (x, y, z) = (-0.5, -0.5, -0.5)

		//	// Vertex 7
		//	-0.5f, -0.5f, 0.5f, // (x, y, z) = (-0.5, -0.5, 0.5)
		//};
		//std::vector<int>indices =
		//{
		//	0, 1, 2, // Front face: Triangle 1 (CCW)
		//	0, 2, 3, // Front face: Triangle 2 (CCW)
		//
		//	4, 6, 5, // Back face: Triangle 3 (CCW)
		//	4, 7, 6, // Back face: Triangle 4 (CCW)
		//
		//	4, 5, 1, // Left face: Triangle 5 (CCW)
		//	4, 1, 0, // Left face: Triangle 6 (CCW)
		//
		//	3, 2, 6, // Right face: Triangle 7 (CCW)
		//	3, 6, 7, // Right face: Triangle 8 (CCW)
		//
		//	1, 5, 6, // Top face: Triangle 9 (CCW)
		//	1, 6, 2, // Top face: Triangle 10 (CCW)
		//
		//	4, 0, 3, // Bottom face: Triangle 11 (CCW)
		//	4, 3, 7, // Bottom face: Triangle 12 (CCW)
		//};
		//triangle.MaterialIndex = 0;
		//Mesh mesh1(verts,indices);
		//m_Scene.Models.push_back(MeshModel(mesh1));
	}
	//{
	//	//in CCW
	//	//plane
	//	std::vector<float>verts =
	//	{
	//		// Triangle 1 (CCW)
	//		2.5f,  -1.f, 2.5f,   // Vertex 0
	//		2.5f,  -1.f, -2.5f,  // Vertex 1
	//		-2.5f, -1.f, -2.5f, // Vertex 2

	//		// Triangle 2 (CCW)
	//		2.5f,  -1.f, 2.5f,   // Vertex 0
	//		-2.5f, -1.f, -2.5f, // Vertex 2
	//		-2.5f, -1.f, 2.5f,  // Vertex 3
	//	};
	//	//triangle.MaterialIndex = 0;
	//	Mesh mesh1(verts);
	//	m_Scene.Meshes.push_back(mesh1);
	//}
};

bool HoveringOnTitlebar = false;

void EditorLayer::OnUIRender()
{
	if (ImGui::BeginViewportSideBar("##TopStatusBar", NULL, ImGuiDir_Up, ImGui::GetFrameHeight(), m_windowflags)) {
		if (ImGui::BeginMenuBar()) {
			ImGui::Image((void*)application::Get().guitexidlist[4], { 24,24 });
			ImGui::Text("MiniRay(prototype)");
			ImGui::InvisibleButton("handlebar", { ImGui::GetWindowWidth() - 270,100 });

			int count = IM_ARRAYSIZE(io.MouseDown);
			int mouse = -1;//right click?

			for (int i = 0; i < count; i++)
				if (ImGui::IsMouseDown(i))
				{
					mouse = i;
				}

			static int winxpos = 0, winypos = 0;
			static bool maximisewindow = false;

			GetMonitorInfo(application::Get().m_HMonitor, &application::Get().m_HMonInfo);

			if (!(mouse == 0))HoveringOnTitlebar = ImGui::IsItemHovered();
			if (mouse == 0 && HoveringOnTitlebar)
			{
				if (application::Get().m_Maximised)
				{
					glfwSetWindowSize(application::Get().GetWindowHandle(), 1600, 900);
					ImVec2 winmouserelativepos{ io.MousePos.x / application::Get().m_HMonInfo.rcWork.right , io.MousePos.y / application::Get().m_HMonInfo.rcWork.bottom };
					glfwSetWindowPos(application::Get().GetWindowHandle(), io.MousePos.x - winmouserelativepos.x * 1600, io.MousePos.y - winmouserelativepos.y * 900);//cursor needs to maintain relative pos with window
					application::Get().m_Maximised = false;
				}
				else if (io.MousePos.y == 0) maximisewindow = true;

				glfwGetWindowPos(application::Get().GetWindowHandle(), &winxpos, &winypos);
				glfwSetWindowPos(application::Get().GetWindowHandle(), io.MouseDelta.x + winxpos, io.MouseDelta.y + winypos);
				if (winxpos == io.MousePos.x && winypos == io.MousePos.y)HoveringOnTitlebar = ImGui::IsItemHovered();
			}
			else if (maximisewindow)
			{
				glfwSetWindowPos(application::Get().GetWindowHandle(), 0, 0);
				glfwSetWindowSize(application::Get().GetWindowHandle(), application::Get().m_HMonInfo.rcWork.right, application::Get().m_HMonInfo.rcWork.bottom - 1);
				maximisewindow = false;
				application::Get().m_Maximised = true;
			}

			ImGui::SetCursorScreenPos({ ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x - 20, ImGui::GetCursorScreenPos().y });
			//minimise button
			if (ImGui::ImageButton((void*)application::Get().guitexidlist[0], { 16,16 })) {
				g_ApplicationRunning = false; application::Get().close();
			};

			ImGui::SetCursorScreenPos({ ImGui::GetCursorScreenPos().x - 60, ImGui::GetCursorScreenPos().y });

			//maximise button
			if (ImGui::ImageButton((application::Get().m_Maximised) ? (void*)application::Get().guitexidlist[2] : (void*)application::Get().guitexidlist[1], { 16,16 }))
			{
				application::Get().m_Maximised = !application::Get().m_Maximised;
				if (!application::Get().m_Maximised)
				{
					glfwSetWindowPos(application::Get().GetWindowHandle(), 100, 100);
					glfwSetWindowSize(application::Get().GetWindowHandle(), 1600, 900);
					//glfwRestoreWindow(application::Get().GetWindowHandle());
				}
				else
				{
					glfwSetWindowPos(application::Get().GetWindowHandle(), 0, 0);
					glfwSetWindowSize(application::Get().GetWindowHandle(), application::Get().m_HMonInfo.rcWork.right, application::Get().m_HMonInfo.rcWork.bottom - 1);
					//glfwMaximizeWindow(application::Get().GetWindowHandle());
				}
			};

			ImGui::SetCursorScreenPos({ ImGui::GetCursorScreenPos().x - 60, ImGui::GetCursorScreenPos().y });
			//close button
			if (ImGui::ImageButton((void*)application::Get().guitexidlist[3], { 16,16 }))glfwIconifyWindow(application::Get().GetWindowHandle());

			ImGui::EndMenuBar();
		}
	};

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

	ImGui::InputTextWithHint("###text1", "search actions", m_top_str_buffer, IM_ARRAYSIZE(m_str_buffer));

	ImGui::EndMainMenuBar();

	if (ImGui::BeginViewportSideBar("##MainStatusBar", NULL, ImGuiDir_Down, ImGui::GetFrameHeight(), m_windowflags)) {
		if (ImGui::BeginMenuBar()) {
			ImGui::Text("Status");
			ImGui::SetCursorScreenPos({ ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("v1.0.0 alpha").x, ImGui::GetCursorScreenPos().y });
			ImGui::Text("v1.0.0 alpha");

			ImGui::EndMenuBar();
		}
	};
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), m_dockflags);

	//------------------------------------------------------------------------------------------------------------------------------
	ImGui::Begin("Scene Graph");
	ImGui::InputTextWithHint("###text2", "start typing to search", m_str_buffer, IM_ARRAYSIZE(m_str_buffer));

	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth;
	//ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

	// 'node_clicked' is temporary storage of what node we have clicked to process selection at the end
	/// of the loop. May be a pointer to your own node type, etc.
	//static int selection_mask = (1 << 2);
	static int selection_mask = (1);
	//static int selection_mask = -1;
	int node_clicked = -1;
	//Sphere* selected_object = nullptr;

	float item_spacing_y = ImGui::GetStyle().ItemSpacing.y;
	float item_offset_y = -item_spacing_y * 0.5f;
	DrawRowsBackground(m_Scene.Models.size() + 5, ImGui::GetTextLineHeight() + item_spacing_y, ImGui::GetCurrentWindow()->WorkRect.Min.x, ImGui::GetCurrentWindow()->WorkRect.Max.x, item_offset_y, 0, ImGui::GetColorU32(ImVec4(0.2f, 0.2f, 0.2f, 0.4f)));

	std::vector<std::string>objnames(m_Scene.Models.size(), "name not found");

	for (int i = 0; i < m_Scene.Models.size(); i++)
	{
		// Disable the default "open on single-click behavior" + set Selected flag according to our selection.
		// To alter selection we use IsItemClicked() && !IsItemToggledOpen(), so clicking on an arrow doesn't alter selection.
		ImGuiTreeNodeFlags node_flags = base_flags;
		const bool is_selected = (selection_mask & (1 << i)) != 0;
		if (is_selected)
			node_flags |= ImGuiTreeNodeFlags_Selected;
		auto triangle = m_Scene.Models[i];
		bool duplicatename = std::find(objnames.begin(), objnames.end(), triangle.name) != objnames.end();
		{
			// Items 3..5 are Tree Leaves
			// The only reason we use TreeNode at all is to allow selection of the leaf. Otherwise we can
			// use BulletText() or advance the cursor by GetTreeNodeToLabelSpacing() and call Text().
			node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; //| ImGuiTreeNodeFlags_Bullet;
			if (triangle.name.find(m_str_buffer) != std::string::npos)
				//ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, std::string(sphere.name + "%d").c_str(), i + 1);
				ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags,
					(duplicatename) ?
					std::string(triangle.name + std::to_string(i)).c_str() : triangle.name.c_str());
			if (!duplicatename)objnames[i] = triangle.name;

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
	//ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
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
			auto& sphere = m_Scene.Models[selection_index];
			ImGui::InputTextWithHint("Name", sphere.name.c_str(), sphere.name.data(), 128);
			//ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1f);
			//ImGui::DragFloat("Radius", &sphere.Radius, 0.1f);
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
	ImGui::PushItemWidth(150);
	ImGui::InputTextWithHint("###text3", "Search materials", m_mat_str_buffer, IM_ARRAYSIZE(m_mat_str_buffer));
	ImGui::PopItemWidth();
	DrawRowsBackground(m_Scene.Materials.size() + 5, ImGui::GetTextLineHeight() + item_spacing_y, ImGui::GetCurrentWindow()->WorkRect.Min.x, ImGui::GetCurrentWindow()->WorkRect.Max.x, item_offset_y, 0, ImGui::GetColorU32(ImVec4(0.2f, 0.2f, 0.2f, 0.4f)));
	ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

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
		Material& material = m_Scene.Materials[mat_selection_index];
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