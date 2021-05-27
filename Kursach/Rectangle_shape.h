#pragma once

#include "Rectangle_collision.h"
#include "Draw_rect.h"

// Клас прямокутника, який об'єднує в собі малювання та колізію прямокутника
class Rectangle_shape : public Draw_rect, public Rectangle_collision
{
public:
	Rectangle_shape();
	Rectangle_shape(RenderWindow* w);

	void set_texture(Texture* t);
	void set_position_shape(float x, float y);
	point2D get_position_shape() const;

	void events_obj();
	void draw();
	void update(float time);

	~Rectangle_shape();
};