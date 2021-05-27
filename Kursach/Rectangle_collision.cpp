#include "Rectangle_collision.h"

Rectangle_collision::Rectangle_collision() : shape_collision(), width(0), height(0)
{
	start_point.x = 0;
	start_point.y = 0;
}
Rectangle_collision::Rectangle_collision(point2D p, float w, float h) : shape_collision(), width(w), height(h) { start_point = p; }

float Rectangle_collision::get_width() const { return width; }
float Rectangle_collision::get_height() const { return height; }

point2D Rectangle_collision::get_start_point() const { return start_point; }
point2D Rectangle_collision::get_end_point() const
{
	point2D a;
	a.x = start_point.x + width;
	a.y = start_point.y + height;
	return a;
}

void Rectangle_collision::set_width(float n) { width = n; }
void Rectangle_collision::set_height(float n) { height = n; }
void Rectangle_collision::set_start_point(point2D p) { start_point = p; }
void Rectangle_collision::set_start_point(float x, float y)
{
	start_point.x = x;
	start_point.y = y;
}

void Rectangle_collision::set_end_point(point2D p)
{
	width = p.x - start_point.x;
	height = p.y - start_point.y;
}

void Rectangle_collision::set_end_point(float x, float y)
{
	width = x - start_point.x;
	height = y - start_point.y;
}

bool Rectangle_collision::is_collision(float x, float y)
{
	// Визначення колізії виконується за знаходженням точки в середині прямокутника по стартовій та кінцевій точці
	point2D end_point = get_end_point();
	// Всі точки зміщуються до початку координат
	end_point.x -= start_point.x;
	end_point.y -= start_point.y;

	point2D point;
	point.x = x - start_point.x;
	point.y = y - start_point.y;

	// Визначення колізії точки з прямокутником
	if ((end_point.x > point.x && end_point.y > point.y) && (0 < point.x && 0 < point.y))
		return true;
	else
		return false;
}

Rectangle_collision::~Rectangle_collision() {}