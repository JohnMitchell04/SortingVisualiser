#include "Renderer.h"

void Renderer::OnResize(uint32_t width, uint32_t height) {
	if (m_image)
	{
		if (m_image->GetWidth() == width && m_image->GetHeight() == height)
			return;

		m_image->Resize(width, height);
	}
	else
	{
		m_image = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] m_imageData;
	m_imageData = new uint32_t[width * height];
}

void Renderer::Render() {
	// TODO: Re-write so that this method goes directly into image data

	// Set all values to black
	std::fill(m_imageData, m_imageData + m_image->GetWidth() * m_image->GetHeight(), 0xff000000);

	// Calculate bar width and bar padding
	int barWidth = (int)std::floor((double)m_image->GetWidth() / ((m_renderData.size() * 2) + 1));


	// Loop through bars and set white where they appear in the image
	for (int i = 0; i < m_renderData.size(); i++) {
		// Calculate bar height
		int barHeight = (int)std::floor((double)m_image->GetHeight() * m_renderData[i] / m_renderData.size());

		// Draw the bar starting at the bottom and moving up
		for (int y = 0; y < barHeight; y++) {
			for (int x = 0; x < barWidth; x++) {
				m_imageData[(m_image->GetHeight() - y - 1) * m_image->GetWidth() + barWidth * ((i * 2) + 1) + x] = 0xffffffff;
			}
		}
	}

	m_image->SetData(m_imageData);
}