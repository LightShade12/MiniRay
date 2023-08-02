#include "../EditorLayer.h"
#include "imgui/imgui_ext.h"
#include <algorithm>
#include "GLFW/glfw3.h"
#include "application/scengraph/scenegraph.h"
#include "application/materialeditor/materialeditor.h"
#include "application/sceneconfigurator/sceneconfigurator.h"
#include "application/attributemanager/attributemanager.h"
#include "tinyfiledialogs/tinyfiledialogs.h"

char const* modelimportfilterPatterns[4] = { "*.obj" , "*.gltf" , "*.glb","*.fbx" };
extern bool g_ApplicationRunning;
char const* modelimportpath = "../test/models/";

EditorLayer::EditorLayer()
	:m_camera(45, 01, 100)
{
	Material& material1 = m_Scene.Materials.emplace_back();
	material1.Albedo = { 0.8,0.8,0.8 };
	material1.Roughness = 0.0f;
	material1.EmissionColor = material1.Albedo;
	material1.name = "white mat";
	material1.EmissionPower = 0.0f;

	Material& material2 = m_Scene.Materials.emplace_back();
	material2.Albedo = { 0.8,0.8,0.8 };
	material2.Roughness = 0.0f;
	material2.EmissionColor = material2.Albedo;
	material2.name = "color mat";
	material2.EmissionPower = 0.0f;

	{
		/*MeshModel mesh1("../test/models/cube.glb");
		m_Scene.Models.push_back(mesh1);*/
		MeshModel plane("../test/models/testscene/plane.glb");
		m_Scene.Models.push_back(plane);
		MeshModel cube("../test/models/testscene/cube.glb");
		m_Scene.Models.push_back(cube);
		MeshModel pyramid("../test/models/testscene/pyramid.glb");
		m_Scene.Models.push_back(pyramid);
		MeshModel cylinder("../test/models/testscene/cylinder.glb");
		m_Scene.Models.push_back(cylinder);
		MeshModel icosphere("../test/models/testscene/icosphere.glb");
		m_Scene.Models.push_back(icosphere);
	}
};

void EditorLayer::OnUIRender()
{
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Import Model"))
		{
			modelimportpath = tinyfd_openFileDialog("Import Model", modelimportpath, 4, modelimportfilterPatterns, NULL, false);
			if (modelimportpath)
			{
				m_Scene.Models.push_back(MeshModel(modelimportpath)); m_Renderer.ResetFrameIndex();
			}
		}

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

	if (ImGui::BeginMenu("Edit")) {
		ImGui::MenuItem("Application settings");
		ImGui::MenuItem("Engine settings");
		ImGui::MenuItem("Keybinds");
		ImGui::EndMenu();
	}
	ImGui::MenuItem("View");
	ImGui::MenuItem("Render");
	ImGui::MenuItem("Tools");
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

	//Scenegraph------------------------------------------------------------------------------------------------------------------------------

	drawSceneGraph(this);

	//AttributeManager------------------------------------------------------------------------------------------------------------------------------

	drawAttributeManager(this);

	//scene configurator-------------------------------------------------------------------------------------------------------------------------

	drawSceneConfigurator(this);

	//-------------------------------------------------------------------------------------------------------------------
	//EDITOR
	drawMaterialEditor(this);

	//Vieport-------------------------------------------------------------------------------------------------------------------
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	ImGui::Begin("Render viewport");
	m_viewportWidth = ImGui::GetContentRegionAvail().x;
	m_viewportHeight = ImGui::GetContentRegionAvail().y - ((ImGui::GetContentRegionAvail().y < 22) ? 0 : 22);

	auto image = m_Renderer.GetFinalImage();
	if (image)
		ImGui::Image((void*)image->GetGLTexID(), ImVec2(image->GetWidth(), image->GetHeight()), { 0,1 }, { 1,0 });

	//viewportbar---------------------------------------------------------
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
	//----------------------------------------------------------------------------------------------------------------------------

	Render();
}