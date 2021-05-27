#include "Ball_collision.h"
#include "Point2D.h"
#include <cmath>

Ball_collision::Ball_collision() : shape_collision(), size(0)
{
	pos.x = 0;
	pos.y = 0;
}

// ������� ������� ���糿 �'���
void Ball_collision::set_pos(float x, float y) { pos.x = x; pos.y = y; }

// ������� ������ ���糿 �'���
void Ball_collision::set_size(float n) { size = n; }

// ���������� ��������� ������ ���糿 �'���
float Ball_collision::get_pos_x() const { return pos.x; }
float Ball_collision::get_pos_y() const { return pos.y; }
point2D Ball_collision::get_pos() const { return pos; }

// ���������� ������ ���糿 �'���
float Ball_collision::get_size() const { return size; }

Ball_collision::~Ball_collision() {}

// ������� ��� ���������� ���糿 ����� � �'���
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