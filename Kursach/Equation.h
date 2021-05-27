#pragma once

#include "Ball_shape.h"
#include "Line.h"
#include "Point2D.h"

// Всі математичні операції сховані в окремому просторі імен, щоб уникнути путанини із стандартною бібліотекою
namespace Vojtekh
{
	// Структура математичного рівняння лінії
	struct Equation
	{
		float A, B, C;

		Equation(float a, float b, float c) : A(a), B(b), C(c) {}
		Equation() {}
	};

	// Функція для повернення рівняння лінії по двом точкам
	Equation get_equation_line_on_2_points(point2D a, point2D b)
	{
		// Перенесення вектора на початок координат
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

	// Функція для визначення колізії мяча та лінії
	bool is_collision_ball_line(Ball_shape* a, Line* b)
	{
		// Перевірка чи мяч знаходиться біля лінії, а не просто на прямій десь в іншій частині поля
		if ((b->start.x - a->get_size() < a->get_position_shape().x &&
			b->start.y - a->get_size() < a->get_position_shape().y) &&
			(b->end.x + a->get_size() > a->get_position_shape().x &&
				b->end.y + a->get_size() > a->get_position_shape().y))
		{
			// Створення структури лінії і перенесення її до початку координат
			Line line;
			line.start.x = b->start.x - a->get_position_shape().x;
			line.start.y = b->start.y - a->get_position_shape().y;
			line.end.x = b->end.x - a->get_position_shape().x;
			line.end.y = b->end.y - a->get_position_shape().y;

			// Визначення рівняння лінії(прямої)
			Equation e = get_equation_line_on_2_points(line.start, line.end);

			// Визначення найближчої точки на лінії до кола
			point2D near;
			near.x = -((e.A * e.C) / (e.A*e.A + e.B*e.B));
			near.y = -((e.B * e.C) / (e.A*e.A + e.B*e.B));

			// Визначення відстані до найближчої точки
			float length_to_near_point = sqrt(near.x*near.x + near.y*near.y);

			if (a->get_size() < length_to_near_point)
				// Колізії немає
				return false;
			else
				// Мяч дотикається до лінії 1 або 2 точками
				return true;
		}
		else
			// Колізії немає, бо мяч далеко від лінії
			return false;
	}
}
