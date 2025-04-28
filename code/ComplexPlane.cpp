#include "ComplexPlane.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include <complex>
#include <SFML/Graphics.hpp>

using namespace sf;

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight) {
	m_pixel_size = { pixelWidth, pixelHeight };
	m_aspectRatio = (float)pixelHeight / (float)pixelWidth;
	m_plane_center = { 0, 0 };
	m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
	m_zoomCount = 0;
	m_state = CALCULATING;
	m_vArray.setPrimitiveType(Points);
	m_vArray.resize(pixelWidth * pixelHeight);
}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const {
	target.draw(m_vArray);
}

void ComplexPlane::zoomIn() {  
	m_zoomCount++;
	float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size = { x, y };
	m_state = CALCULATING;
}

void ComplexPlane::zoomOut() {  
	m_zoomCount--;
	float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
	float y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
	m_plane_size = { x, y };
	m_state = CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel) { 
	m_plane_center = mapPixelToCoords(mousePixel);
	m_state = CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousPixel) { //might not work; not tested
	m_mouseLocation = mapPixelToCoords(mousPixel);
}

void ComplexPlane::loadText(Text& text) {
	std::stringstream ss;
	ss << "Mandelbrot Set" << '\n'
		<< "Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")" << '\n'
		<< "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")" << '\n'
		<< "Left click to zoom in" << '\n'
		<< "Right click to zoom out" << '\n';

	text.setString(ss.str());
}

void ComplexPlane::updateRender() {
	int pixelWidth = m_pixel_size.x;
	int pixelHeight = m_pixel_size.y;

	if (m_state == CALCULATING) {
		for (int i = 0; i < pixelHeight; i++) {
			for (int j = 0; j < pixelWidth; j++) {
				m_vArray[j + i * pixelWidth].position = { (float)j,(float)i };

				Uint8 r, g, b;
				Vector2i coord(j, i);
				iterationsToRGB(countIterations(mapPixelToCoords(coord)), r, g, b);

				m_vArray[j + i * pixelWidth].color = { r,g,b };

				m_state = DISPLAYING;
			}
		}
	}
}

int ComplexPlane::countIterations(Vector2f coord) {
	std::complex<float> c (coord.x, coord.y);
	std::complex<float> z = 0;

	int i = 0;

	while (abs(z) < 2.0 && i < MAX_ITER)
	{
		z = z * z + c;
		i++;
	}

	return i;
}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b) { 
	int numColors = 4;

	for (int i = MAX_ITER; i > 0; i--) {
		if (count == i) {
			/*r = (i) * 5;
			g = (i) / 390;
			b = (i);*/
			/*r = i / 45;
			g = i / 3;
			b = i / 4;*/
			/*r = (i) * 5;
			g = (i) * 390;
			b = (i) * 9;*/
			/*r = i * 456;
			g = i * 1356;
			b = i * 789;*/
			r = i * 459720734;
			g = i * 1356293872384;
			b = i * 7898130658013;
		}
		if (i % 2 == 0) {
			r *= 789;
		}
		else {
			b *= 686;
		}
	}
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel) { 
	//((n  - a) / (b - a)) * (d - c) + c
	//[a,b]
	Vector2f xRange(0.0, m_pixel_size.x);
	Vector2f yRange(m_pixel_size.y, 0.0);
	//[c,d]
	Vector2f complex((m_plane_center.x - m_plane_size.x / 2.0), (m_plane_center.y - m_plane_size.y / 2.0));
	Vector2f mappedCoor;

	mappedCoor.x = ((float)(mousePixel.x - xRange.x) / (xRange.y - xRange.x) * m_plane_size.x + complex.x);
	mappedCoor.y = ((float)(mousePixel.y - yRange.x) / (yRange.y - yRange.x) * m_plane_size.y + complex.y);

	return mappedCoor;
}