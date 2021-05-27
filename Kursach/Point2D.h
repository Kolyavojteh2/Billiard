#pragma once

// Структура точки, аналог sf::Vector2f
struct point2D
{
	float x, y;

	point2D() : x(0), y(0) {}
	point2D(float a, float b) : x(a), y(b) {}
};