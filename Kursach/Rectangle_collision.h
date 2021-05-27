#pragma once

#include "shape_collision.h"
#include "Point2D.h"

// ���� ���糿 ������������
class Rectangle_collision : public shape_collision
{
private:
	// �� ��� ���� �� �������� ����� �� ������ � ������
	point2D start_point;
	float width;
	float height;

public:
	Rectangle_collision();
	Rectangle_collision(point2D p, float w, float h);

	// ���������� ������� �� ������ ���糿 ������������
	float get_width() const;
	float get_height() const;

	// ���������� �������� �� ������ ����� ���糿 ������������
	point2D get_start_point() const;
	point2D get_end_point() const;

	// ������� ��������� ���糿 ������������
	void set_width(float n);
	void set_height(float n);
	void set_start_point(point2D p);
	void set_start_point(float x, float y);
	void set_end_point(point2D p);
	void set_end_point(float x, float y);

	// �������������� ������� ���糿
	bool is_collision(float x, float y);

	~Rectangle_collision();
};