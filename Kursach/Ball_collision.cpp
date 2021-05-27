#include "Ball_collision.h"
#include "Point2D.h"
#include <cmath>

Ball_collision::Ball_collision() : shape_collision(), size(0)
{
	pos.x = 0;
	pos.y = 0;
}

// Задання позиції колізії м'яча
void Ball_collision::set_pos(float x, float y) { pos.x = x; pos.y = y; }

// Задання розміру колізії м'яча
void Ball_collision::set_size(float n) { size = n; }

// Повернення координат центра колізії м'яча
float Ball_collision::get_pos_x() const { return pos.x; }
float Ball_collision::get_pos_y() const { return pos.y; }
point2D Ball_collision::get_pos() const { return pos; }

// Повернення розміру колізії м'яча
float Ball_collision::get_size() const { return size; }

Ball_collision::~Ball_collision() {}

// Функція для визначення колізії точки і м'яча
bool Ball_collision::is_collision(float x, float y)
{
	point2D ball;
	ball.x = pos.x + 8;
	ball.y = pos.y + 40;

	point2D point;
	point.x = x - ball.x;
	point.y = y - ball.y;

	ball.x = 0;
	ball.y = 0;

	float radius_point = sqrt(point.x*point.x + point.y*point.y);

	if (radius_point <= this->size)
		return true;
	else
		return false;
}