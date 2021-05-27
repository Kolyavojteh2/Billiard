#pragma once

#include "Point2D.h"
// Всі математичні операції сховані в окремому просторі імен, щоб уникнути путанини із стандартною бібліотекою
namespace Vojtekh
{
	class Vector_rad
	{
	private:
		float angle;	// кут
		float value;	// число(сила)

	public:
		Vector_rad();
		Vector_rad(float a, float v);

		void set_vector(Vector_rad);
		void set_vector(float a, float v);

		void set_angle(float n);
		void set_value(float n);

		float get_angle() const;
		float get_value() const;

		float get_proection_X() const;
		float get_proection_Y() const;

		Vector_rad operator+(Vector_rad b);

		Vector_rad operator*(float b);

		Vector_rad operator/(float b);
		
		bool operator==(Vector_rad b);
		bool operator!=(Vector_rad b);
				
		void normalize_vect();		// нормалізація вектора
		void reverse_direct();		// реверсування напрямку

		friend bool is_collinear(Vector_rad a, Vector_rad b);	// перевірка на колінеарність

		~Vector_rad();
	};

	// Функція для визначення кута за косинусом та синусом
	float get_angle(float sin, float cos);

	// Функція для визначення кута за двома точками
	float get_angle_2_points(point2D A, point2D B);
}
