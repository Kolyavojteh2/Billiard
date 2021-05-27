#pragma once

#include "Ball_shape.h"
#include "Line.h"
#include "Point2D.h"

// �� ���������� �������� ������ � �������� ������� ����, ��� �������� �������� �� ����������� ���������
namespace Vojtekh
{
	// ��������� ������������� ������� ��
	struct Equation
	{
		float A, B, C;

		Equation(float a, float b, float c) : A(a), B(b), C(c) {}
		Equation() {}
	};

	// ������� ��� ���������� ������� �� �� ���� ������
	Equation get_equation_line_on_2_points(point2D a, point2D b)
	{
		// ����������� ������� �� ������� ���������
		point2D vectorAB;
		vectorAB.x = b.x - a.x;
		vectorAB.y = b.y - a.y;

		float dy = vectorAB.x / vectorAB.y;

		Equation result;

		result.A = dy;
		result.B = -1;
		result.C = a.x - a.y*dy;

		return result;
	}

	// ������� ��� ���������� ���糿 ���� �� ��
	bool is_collision_ball_line(Ball_shape* a, Line* b)
	{
		// �������� �� ��� ����������� ��� ��, � �� ������ �� ����� ���� � ����� ������ ����
		if ((b->start.x - a->get_size() < a->get_position_shape().x &&
			b->start.y - a->get_size() < a->get_position_shape().y) &&
			(b->end.x + a->get_size() > a->get_position_shape().x &&
				b->end.y + a->get_size() > a->get_position_shape().y))
		{
			// ��������� ��������� �� � ����������� �� �� ������� ���������
			Line line;
			line.start.x = b->start.x - a->get_position_shape().x;
			line.start.y = b->start.y - a->get_position_shape().y;
			line.end.x = b->end.x - a->get_position_shape().x;
			line.end.y = b->end.y - a->get_position_shape().y;

			// ���������� ������� ��(�����)
			Equation e = get_equation_line_on_2_points(line.start, line.end);

			// ���������� ��������� ����� �� �� �� ����
			point2D near;
			near.x = -((e.A * e.C) / (e.A*e.A + e.B*e.B));
			near.y = -((e.B * e.C) / (e.A*e.A + e.B*e.B));

			// ���������� ������ �� ��������� �����
			float length_to_near_point = sqrt(near.x*near.x + near.y*near.y);

			if (a->get_size() < length_to_near_point)
				// ���糿 ����
				return false;
			else
				// ��� ���������� �� �� 1 ��� 2 �������
				return true;
		}
		else
			// ���糿 ����, �� ��� ������ �� ��
			return false;
	}
}
