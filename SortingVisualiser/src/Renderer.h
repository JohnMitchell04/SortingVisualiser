#pragma once

#include "Walnut/Image.h"

#include <memory>
#include <vector>
#include <cmath>
#include <mutex>

class Renderer {
public:
	Renderer() : m_renderData(100, 0) {};

	void SetRenderData(std::vector<int> renderData) { m_renderData = renderData; }

	std::shared_ptr<Walnut::Image> GetImage() const { return m_image; }

	void OnResize(uint32_t width, uint32_t height);
	void Render();

private:
	std::shared_ptr<Walnut::Image> m_image;
	uint32_t* m_imageData = nullptr;
	std::vector<int> m_renderData;
};