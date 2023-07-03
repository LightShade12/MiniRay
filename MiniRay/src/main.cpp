#include "application/application.h"
#include "application/entrypoint.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include <iostream>
#include "glm/glm.hpp"

//rename file to other than main.cpp
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

class ViewportLayer : public Layer
{
public:

	virtual void OnUIRender() override
	{
		ImGui::Begin("Viewport");

		ImGui::End();
	}
};

class NodeInspectorLayer : public Layer
{
public:

	virtual void OnUIRender() override
	{
		ImGui::Begin("Inspector");
		ImGui::Button("Button");
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

class AttributeManagerLayer : public Layer
{
public:

	virtual void OnUIRender() override
	{
		ImGui::Begin("Attribute Manager");
		if (ImGui::CollapsingHeader("Object Properties"))
		{
		}
		ImGui::End();
	}
};

class EditorLayer : public Layer
{
public:

	virtual void OnUIRender() override
	{
		ImGui::Begin("Editor");
		//ImGui::Button("Button");
		ImGui::End();
	}
};

class DevWindowLayer : public Layer
{
public:

	virtual void OnUIRender() override
	{
		ImGui::ShowDemoWindow();
	}
};

application* create_application(int argc, char* argv[]) {
	std::cout << "Running from: " << argv[0] << "\n";
	application_specification spec;
	spec.name = "test app";
	spec.iconpath = "resources/vgui/appicon.png";
	auto app = new application(spec);

	app->PushLayer<DockspaceLayer>();
	app->PushLayer<ViewportLayer>();
	app->PushLayer<NodeInspectorLayer>();
	app->PushLayer<SceneGraphLayer>();
	app->PushLayer<EditorLayer>();
	app->PushLayer<AttributeManagerLayer>();

	return app;
}