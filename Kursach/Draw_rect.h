#pragma once

#include <SFML/Graphics.hpp>
#include "Point2D.h"

using namespace sf;

// Основний клас для малювання текстур у вікні
class Draw_rect
{
private:
	Sprite sprite;

	RenderWindow* window;	// вікно для малювання

public:
	Draw_rect();
	Draw_rect(RenderWindow* w);

	RenderWindow* get_RenderWindow() const;
	point2D get_position_draw_rect() const;
	float get_rotate_angle() const;

	float get_height() const;
	float get_width() const;

	void set_window(RenderWindow* w);
	void set_position_draw_rect(int x, int y);
	void set_rotate_angle(float angle);
	void set_texture(Texture* t);

	void move(float x, float y);
	void draw();

	~Draw_rect();
};