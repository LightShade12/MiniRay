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
		{
			Sphere sphere;
			sphere.Position = { 0,0,0 };
			sphere.albedo = { 1,1,1 };
			sphere.Radius = 0.5f;
			m_scene.Spheres.push_back(sphere);
		}
		{
			Sphere sphere;
			sphere.Position = { 1,0,0 };
			sphere.albedo = { 1,0,1 };
			sphere.Radius = 0.5f;
			m_scene.Spheres.push_back(sphere);
		}
	};

	virtual void OnUpdate(float ts) {
		m_camera.OnUpdate(ts);
	}

	virtual void OnAttach() override
	{
		//m_image = std::make_shared<Image>("../test/textures/try.png");
	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("settings");
		ImGui::Text("last render time: %.3fms", m_lastrendertime);
		/*ImGui::DragFloat3("Position", glm::value_ptr(m_scene.Spheres[0].Position), 0.1f);
		ImGui::DragFloat("Radius", &m_scene.Spheres[0].Radius, 0.1f);*/
		
		for (size_t i = 0; i < m_scene.Spheres.size(); i++)
		{
			ImGui::PushID(i);

			Sphere& sphere = m_scene.Spheres[i];
			ImGui::DragFloat3("Position", glm::value_ptr(sphere.Position), 0.1f);
			ImGui::DragFloat("Radius", &sphere.Radius, 0.1f);
			ImGui::ColorEdit3("Albedo", glm::value_ptr(sphere.albedo));
			//ImGui::DragInt("Material", &sphere.MaterialIndex, 1.0f, 0, (int)m_Scene.Materials.size() - 1);

			ImGui::Separator();

			ImGui::PopID();
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		ImGui::Begin("Viewport");
		m_viewportWidth = ImGui::GetContentRegionAvail().x;
		m_viewportHeight = ImGui::GetContentRegionAvail().y;

		auto image = m_renderer.GetFinalImage();
		if (image)
			ImGui::Image((void*)image->GetGLTexID(), ImVec2(image->GetWidth(), image->GetHeight()), { 0,1 }, { 1,0 });
		ImGui::End();

		ImGui::PopStyleVar();

		Render();
	}

	void Render() {
		Timer timer;
		m_renderer.OnResize(m_viewportWidth, m_viewportHeight);
		m_camera.OnResize(m_viewportWidth, m_viewportHeight);

		m_renderer.render(m_scene, m_camera);

		m_lastrendertime = timer.ElapsedMillis();
	};
	Scene m_scene;
	Camera m_camera;
	float m_lastrendertime = 0;
	renderer m_renderer;
	uint32_t m_viewportWidth = 0, m_viewportHeight = 0;
};