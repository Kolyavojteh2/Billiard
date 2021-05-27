#pragma once

#include "shape_collision.h"
#include "Point2D.h"

// Клас колізії прямокутника
class Rectangle_collision : public shape_collision
{
private:
	// Має такі поля як стартова точка та ширина і висота
	point2D start_point;
	float width;
	float height;

public:
	Rectangle_collision();
	Rectangle_collision(point2D p, float w, float h);

	// Повернення шириини та висоти колізії прямокутника
	float get_width() const;
	float get_height() const;

	// Повернення стартової та кінцевої точки колізії прямокутника
	point2D get_start_point() const;
	point2D get_end_point() const;

	// Задання параметрів колізії прямокутника
	void set_width(float n);
	void set_height(float n);
	void set_start_point(point2D p);
	void set_start_point(float x, float y);
	void set_end_point(point2D p);
	void set_end_point(float x, float y);

	// Перевизначення функції колізії
	bool is_collision(float x, float y);

	~Rectangle_collision();
};