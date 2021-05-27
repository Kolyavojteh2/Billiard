#include "Vector_rad.h"
#include <cmath>
#include <iostream>

const float PI = 3.14159265;

// Всі математичні операції сховані в окремому просторі імен, щоб уникнути путанини із стандартною бібліотекою
namespace Vojtekh
{
	// Конструктор за замовчуванням та явний конструктор з значеннями кута та числа(сили)
	Vector_rad::Vector_rad() : angle(0), value(0) {}
	Vector_rad::Vector_rad(float a, float v) : angle(a), value(v) {}

	// Копіювання вектора
	void Vector_rad::set_vector(Vector_rad b) { *this = b; }
	void Vector_rad::set_vector(float a, float v) { angle = a; value = v; }

	// Функції для запису та повернення значеннь класу
	void Vector_rad::set_angle(float n) { angle = n; }
	void Vector_rad::set_value(float n) { value = n; }
	float Vector_rad::get_angle() const { return angle; }
	float Vector_rad::get_value() const { return value; }

	// Повернення значень сили по проекціям на вісі
	float Vector_rad::get_proection_X() const { return value * cos(angle); }
	float Vector_rad::get_proection_Y() const { return value * sin(angle); }

	// Додавання двох векторів
	Vector_rad Vector_rad::operator+(Vector_rad b) 
	{
		Vector_rad result;

		// Створення точки В від першого вектора (а)
		point2D B;
		B.x = get_proection_X();
		B.y = get_proection_Y();
		
		// Створення точки С від другого вектора (b)
		point2D C;
		C.x = B.x + b.get_proection_X();
		C.y = B.y + b.get_proection_Y();

		// Визначення сили результуючого вектора
		result.set_value(sqrt(C.x*C.x + C.y*C.y));

		// Спрощення розрахунку, якщо значення дорівнює 0 то і кут ставимо теж нуль
		if (result.get_value() == 0.0)
		{
			result.set_angle(0);
		}
		else
		{
			// Якщо сила не нульова, то визначаємо кут
			float sin_a, cos_a;
			cos_a = C.x / result.get_value();
			sin_a = C.y / result.get_value();

			result.set_angle(Vojtekh::get_angle(sin_a, cos_a));
		}
		return result;
	}

	// Множення вектора на константу, просто домножаємо значення сили на константу
	Vector_rad Vector_rad::operator*(float b) 
	{ 
		Vector_rad v;
		v.set_value(value * b); 
		v.set_angle(angle);

		return v;
	}

	// Ділення вектора на константу, виконується діленням сили на константу
	Vector_rad Vector_rad::operator/(float b) 
	{ 
		Vector_rad v;
		v.set_value(value / b);
		v.set_angle(angle);

		return v;
	}
	
	// Операції ідентичності векторів
	bool Vector_rad::operator==(Vector_rad b)
	{
		if (value == b.value && angle == b.angle)
			return true;
		else
			return false;
	}
	bool Vector_rad::operator!=(Vector_rad b)
	{
		return !(*this == b);
	}
	
	// Нормалізація вектора
	void Vector_rad::normalize_vect()
	{
		// Якщо значення менше нуля, то ми просто повертаємо вектор в протилежну сторону і беремо значення його по модулю
		if (value < 0)
		{
			angle += PI;
			value = abs(value);
		}

		// Якщо значення кута від'ємне то додаємо до кута 360 градусів(2п)
		while (angle < 0)
			angle += 2 * PI;

		// Якщо значення кута більше за 360 градусів(2п) то віднімаємо від кута 360 градусів(2п)
		while (angle > (2 * PI))
			angle -= 2 * PI;
	}

	// Зміна напрямку вектора на протилежний
	void Vector_rad::reverse_direct()
	{
		angle += 180.0;
		normalize_vect();
	}

	// Перевірка на колінеарність векторів
	/*friend*/ bool is_collinear(Vector_rad a, Vector_rad b)
	{
		if (a.angle == b.angle || a.angle == (-b.angle))
			return true;
		else
			return false;
	}

	// Визначення кута за косинусами та синусами
	float get_angle(float sin, float cos)
	{
		int quartet = 0;

		if (((0 < cos) && (cos < 1)) && ((0 < sin) && (sin < 1)))
			quartet = 1;
		if (((-1 < cos) && (cos < 0)) && ((0 < sin) && (sin < 1)))
			quartet = 2;
		if (((-1 < cos) && (cos < 0)) && ((-1 < sin) && (sin < 0)))
			quartet = 3;
		if (((0 < cos) && (cos < 1)) && ((-1 < sin) && (sin < 0)))
			quartet = 4;

		if (cos == 1)
			quartet = 10;
		if (sin == 1)
			quartet = 20;
		if (cos == -1)
			quartet = 30;
		if (sin == -1)
			quartet = 40;

		float angle;

		switch (quartet)
		{
		case 1:
		{
			//angle = acos(cos);
			angle = acos(cos);
			break;
		}
		case 2:
		{
			//angle = asin(cos) + (PI/2);
			angle = acos(cos);
			break;
		}
		case 3:
		{
			//angle = asin(sin) + PI;
			//angle = asin(sin);
			angle = -acos(cos);
			break;
		}
		case 4:
		{
			//angle = acos(cos); //+ (PI / 2) * 3;
			angle = asin(sin);
			break;
		}

		case 10: 
		{
			angle = acos(cos);
			break;
		}
		case 20:
		{
			angle = asin(sin);
			break;
		}
		case 30:
		{
			angle = acos(cos);
			break;
		}
		case 40:
		{
			angle = asin(sin);
			break;
		}

		default:
			break;
		}

		return angle;
	}

	Vector_rad::~Vector_rad() {}

	// Визначення кута за двома точками за правилом трикутника 
	float get_angle_2_points(point2D A, point2D B)
	{
		point2D B1;
		B1.x = B.x - A.x;
		B1.y = B.y - A.y;

		float size = sqrt(B1.x*B1.x + B1.y*B1.y);

		float sin, cos;
		sin = B1.y / size;
		cos = B1.x / size;

		return get_angle(sin, cos);
	}
}

