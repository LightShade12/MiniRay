#pragma once

#include "application/core/intern/layer/layer.h"
#include "application/core/intern/image/image.h"
#include "imgui/imgui.h"
#include "application/console/console.h"
#include "application/outputlogger/outputlogger.h"

class DevWindowLayer : public Layer
{
public:

	virtual void OnAttach() override
	{
		log.AddLog("[%05d] [info] Initialised %.1f\n",
			ImGui::GetFrameCount(), ImGui::GetTime());
	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("Dev window");
		if (ImGui::BeginTabBar("MyTabBar", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Console"))
			{
				console.rawDraw();
				ImGui::EndTabItem();
			}
			//-------------------------------------------------
			if (ImGui::BeginTabItem("OutPut"))
			{
				if (ImGui::SmallButton("[Debug] Add 5 entries"))
				{
					static int counter = 0;
					const char* categories[3] = { "info", "warn", "error" };
					const char* words[] = { "Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate", "Nincompoop", "Pauciloquent" };
					for (int n = 0; n < 5; n++)
					{
						const char* category = categories[counter % IM_ARRAYSIZE(categories)];
						const char* word = words[counter % IM_ARRAYSIZE(words)];
						log.AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
							ImGui::GetFrameCount(), category, ImGui::GetTime(), word);
						counter++;
					}
				}
				log.rawDraw();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
		//ImGui::ShowDemoWindow();
		ImGui::End();
	}
	ExampleAppConsole console;
	ExampleAppLog log;
};
