#include <chrono>
#include <thread>

#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include "Sorter.h"
#include "Renderer.h"

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Control Panel");

		ImGui::Text("Last Render: %.3fms", m_lastRenderTime);
		ImGui::Text("Comparisons: %d", m_sorter.GetComparisons());
		ImGui::Text("Array Accesses: %d", m_sorter.GetAccesses());

		if (ImGui::Button("Selection Sort")) {
			m_sorter.SetAlgorithm(Sorting::SortingAlgorithms::Selection);

			// Randomise data
			m_sorter.RandomiseData();

			// Update render data
			m_renderer.SetRenderData(m_sorter.GetData());

			while (m_sorter.IsSorting()) {
				m_sorter.NextStep();

				// Update render data
				m_renderer.SetRenderData(m_sorter.GetData());
			}
		}

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_viewportWidth = ImGui::GetContentRegionAvail().x;
		m_viewportHeight = ImGui::GetContentRegionAvail().y;

		auto image = m_renderer.GetImage();
		if (image) 
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(), (float)image->GetHeight() });

		ImGui::End();
		ImGui::PopStyleVar();

		Render();
	}

	void Render() {
		Walnut::Timer timer;

		m_renderer.OnResize(m_viewportWidth, m_viewportHeight);
		m_renderer.Render();

		m_lastRenderTime = timer.ElapsedMillis();
	}

private:
	Sorting::Sorter m_sorter;
	Renderer m_renderer;

	uint32_t m_viewportWidth = 0, m_viewportHeight = 0;
	float m_lastRenderTime = 0.0f;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Sorting Visualisation";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}