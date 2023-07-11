#include "application/core/application.h"
#include "entrypoint.h"
#include "../extern/imgui/imgui.h"
#include "../extern/imgui/imgui_internal.h"
#include <iostream>
#include "glm/glm.hpp"
#include "application/console/console.h"
#include "application/outputlogger/outputlogger.h"
#include "application/core/intern/image/image.h"
#include "opengl/glutils.h"
#include "application/EditorLayer/EditorLayer.h"
#include "application/nodeinspector/nodeinspector.h"
#include "application/attributemanager/attributemanager.h"
#include "application/devwindow/devwindow.h"

//to be implemented by client

class DockspaceLayer : public Layer
{
public:

	virtual void OnUIRender() override
	{
		ImGui::BeginMainMenuBar();
		ImGui::MenuItem("File");
		ImGui::MenuItem("Edit");
		ImGui::MenuItem("View");
		ImGui::MenuItem("Window");
		ImGui::MenuItem("Help");
		ImGui::InputTextWithHint("", "search actions", m_str_buffer, IM_ARRAYSIZE(m_str_buffer));
		ImGui::EndMainMenuBar();
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), m_dockflags);
		if (ImGui::BeginViewportSideBar("##MainStatusBar", NULL, ImGuiDir_Down, ImGui::GetFrameHeight(), m_windowflags)) {
			if (ImGui::BeginMenuBar()) {
				ImGui::Text("Status");
				ImGui::EndMenuBar();
			}
		};
	};
	char m_str_buffer[128] = "";
	ImGuiDockNodeFlags m_dockflags = ImGuiDockNodeFlags_PassthruCentralNode;
	ImGuiWindowFlags m_windowflags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
};

class SceneGraphLayer : public Layer
{
public:

	virtual void OnUIRender() override
	{
		ImGui::Begin("Scene Graph");
		ImGui::InputTextWithHint("", "start typing to search", m_str_buffer, IM_ARRAYSIZE(m_str_buffer));
		ImGui::End();
	}
	char m_str_buffer[128] = "";
};

class NodeEditorLayer : public Layer
{
public:

	virtual void OnUIRender() override
	{
		ImGui::Begin("Editor");
		//ImGui::Button("Button");
		ImGui::End();
	}
};

application* create_application(int argc, char* argv[]) {
	std::cout << "Running from: " << argv[0] << "\n";
	application_specification spec;
	spec.name = "test app";
	spec.iconpath = "resources/vgui/appicon.png";
	auto app = new application(spec);

	//app->PushLayer<DockspaceLayer>();
	app->PushLayer<EditorLayer>();
	//app->PushLayer<NodeInspectorLayer>();
	//app->PushLayer<SceneGraphLayer>();
	//app->PushLayer<NodeEditorLayer>();
	//app->PushLayer<AttributeManagerLayer>();
	app->PushLayer<DevWindowLayer>();

	return app;
}