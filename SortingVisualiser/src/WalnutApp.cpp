#include <chrono>
#include <thread>
#include <atomic>

#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include "Sorter.h"
#include "Renderer.h"

std::mutex renderDataMutex;
std::mutex cancelMutex;
std::condition_variable conditional;

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Control Panel");

		ImGui::Text("Last Render: %.3fms", m_lastRenderTime);
		ImGui::Text("Total Sort Time: %.3fms", m_totalSortTime);
		ImGui::Text("Comparisons: %d", m_sorter.GetComparisons());
		ImGui::Text("Array Accesses: %d", m_sorter.GetAccesses());

		ImGui::SliderInt("Items to sort", &m_nItems, 1, 1000);

		if (ImGui::Button("Selection Sort")) {
			CheckSorting();
			m_sorter.SetAlgorithm(Sorting::SortingAlgorithms::Selection);
			HandleSorting();
		}

		if (ImGui::Button("Insertion Sort")) {
			CheckSorting();
			m_sorter.SetAlgorithm(Sorting::SortingAlgorithms::Insertion);
			HandleSorting();
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

		// Ensure we lock render data when rendering
		renderDataMutex.lock();
		m_renderer.Render();
		renderDataMutex.unlock();

		m_lastRenderTime = timer.ElapsedMillis();
	}

	void CheckSorting() {
		// Ensure sorter is not currently sorting before starting
		// If sorter is sorting, notify detached thread have it exit and wait
		if (m_sorter.IsSorting()) {
			m_waitingAlgo = true;
			std::unique_lock<std::mutex> lock(cancelMutex);
			conditional.wait(lock);
		}
	}

	void HandleSorting() {
		// Reset sorting time
		m_totalSortTime = 0.0f;

		// Randomise data
		m_sorter.RandomiseData(m_nItems);

		// Update render data
		m_renderer.SetRenderData(m_sorter.GetData());

		// This will be exectued on a seperate thread so that updating render data can be slowed without affecting UI rendering
		std::thread sortingThread([&]() {
			while (m_sorter.IsSorting()) {
				if (m_waitingAlgo) {
					std::lock_guard<std::mutex> lock(cancelMutex);
					m_waitingAlgo = false;
					conditional.notify_all();
					return;
				}

				// Sleep so user can see step
				std::this_thread::sleep_for(std::chrono::milliseconds(10));

				// Time and perform next sorting step
				Walnut::Timer timer;
				m_sorter.NextStep();
				m_totalSortTime += timer.ElapsedMillis();

				// Update render data
				m_renderer.SetRenderData(m_sorter.GetData());
			}
		});

		// Detach thread so it can execute concurrently
		sortingThread.detach();
	}

private:
	Sorting::Sorter m_sorter;
	Renderer m_renderer;
	std::atomic<bool> m_waitingAlgo = false;

	uint32_t m_viewportWidth = 0, m_viewportHeight = 0;
	float m_lastRenderTime = 0.0f;
	float m_totalSortTime = 0.0f;
	int m_nItems;
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