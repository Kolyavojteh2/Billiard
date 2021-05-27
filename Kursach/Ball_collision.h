#pragma once

#include "shape_collision.h"
#include "Point2D.h"

//  лас кол≥з≥њ м€ча
class Ball_collision : public shape_collision
{
	point2D pos;
	float size;			// розм≥р м€ча

public:
	Ball_collision();

	void set_pos(float x, float y);
	void set_size(float n);

	float get_pos_x() const;
	float get_pos_y() const;
	point2D get_pos() const;
	float get_size() const;

	bool is_collision(float x, float y);

	~Ball_collision();
};