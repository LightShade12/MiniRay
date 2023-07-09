#pragma once
#include <memory>
#include "application/core/intern/layer/layer.h"
#include "application/core/intern/image/image.h"
#include "application/core/intern/timer/timer.h"
#include "application/renderer/renderer.h"
#include "application/camera/camera.h"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "glm/glm.hpp"

class ViewportLayer : public Layer
{
public:

	ViewportLayer() :m_camera(45, 01, 100)
	{
		Material& pinkSphere = m_Scene.Materials.emplace_back();
		pinkSphere.Albedo = { 1.0f, 0.0f, 1.0f };
		pinkSphere.Roughness = 0.0f;

		Material& blueSphere = m_Scene.Materials.emplace_back();
		blueSphere.Albedo = { 0.2f, 0.3f, 1.0f };
		blueSphere.Roughness = 0.1f;

		//Material& orangeSphere = m_Scene.Materials.emplace_back();
		//orangeSphere.Albedo = { 0.8f, 0.5f, 0.2f };
		//orangeSphere.Roughness = 0.1f;
		//orangeSphere.EmissionColor = orangeSphere.Albedo;
		//orangeSphere.EmissionPower = 2.0f;

		{
			Sphere sphere;
			sphere.Position = { 0.0f, 0.0f, 0.0f };
			sphere.Radius = 1.0f;
			sphere.MaterialIndex = 0;
			m_Scene.Spheres.push_back(sphere);
		}

		/*{
			Sphere sphere;
			sphere.Position = { 2.0f, 0.0f, 0.0f };
			sphere.Radius = 1.0f;
			sphere.MaterialIndex = 2;
			m_Scene.Spheres.push_back(sphere);
		}*/

		{
			Sphere sphere;
			sphere.Position = { 0.0f, -101.0f, 0.0f };
			sphere.Radius = 100.0f;
			sphere.MaterialIndex = 1;
			m_Scene.Spheres.push_back(sphere);
		}
	};

	virtual void OnUpdate(float ts) {
		if (m_camera.OnUpdate(ts))
			m_Renderer.ResetFrameIndex();
	}

	virtual void OnAttach() override
	{
		//m_image = std::make_shared<Image>("../test/textures/try.png");
	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("settings");
		ImGui::Text("last render time: %.3fms", m_lastrendertime);
		if (ImGui::Button("Reset buffer"))
			m_Renderer.ResetFrameIndex();
		ImGui::Checkbox("Acumulate", &m_Renderer.GetSettings().Accumulate);


		for (size_t i = 0; i < m_Scene.Spheres.size(); i++)
		{
			ImGui::PushID(i);

			Sphere& sphere = m_Scene.Spheres[i];
			ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1f);
			ImGui::DragFloat("Radius", &sphere.Radius, 0.1f);
			ImGui::DragInt("Material", &sphere.MaterialIndex, 1.0f, 0, (int)m_Scene.Materials.size() - 1);

			ImGui::Separator();

			ImGui::PopID();
		}

		for (size_t i = 0; i < m_Scene.Materials.size(); i++)
		{
			ImGui::PushID(i);

			Material& material = m_Scene.Materials[i];
			ImGui::ColorEdit3("Albedo", glm::value_ptr(material.Albedo));
			ImGui::DragFloat("Roughness", &material.Roughness, 0.05f, 0.0f, 1.0f);
			ImGui::DragFloat("Metallic", &material.Metallic, 0.05f, 0.0f, 1.0f);
			//ImGui::ColorEdit3("Emission Color", glm::value_ptr(material.EmissionColor));
			//ImGui::DragFloat("Emission Power", &material.EmissionPower, 0.05f, 0.0f, FLT_MAX);

			ImGui::Separator();

			ImGui::PopID();
		}
		
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

	void Render() {
		Timer timer;
		m_Renderer.OnResize(m_viewportWidth, m_viewportHeight);
		m_camera.OnResize(m_viewportWidth, m_viewportHeight);

		m_Renderer.render(m_Scene, m_camera);

		m_lastrendertime = timer.ElapsedMillis();
	};
	Scene m_Scene;
	Camera m_camera;
	float m_lastrendertime = 0;
	renderer m_Renderer;
	uint32_t m_viewportWidth = 0, m_viewportHeight = 0;
};