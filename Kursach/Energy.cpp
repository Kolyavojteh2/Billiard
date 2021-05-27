#include "Energy.h"
#include <cmath>

#include <iostream>

using namespace Vojtekh;

Energy::Energy() : Vector_rad() {}
Energy::Energy(float e, float a) : Vector_rad(a, e) {}

// Повернення швидкості об'єкта(клас вектор повертає енергію об'єкта)
float Energy::get_speed() const { return sqrt(get_value()); }

// Задання швидкості об'єкта
void Energy::set_speed(float n) { set_value(n*n); }

// Оновлення значення енергії
// Механіка тертя тіла об поверхню
void Energy::update(float time)
{
	float val = get_value();

	// Якщо енергія нульова ми одразу виходимо з функції
	if (val == 0)
		return;

	// При великій кількості енергії тіла енергія втрачається у звичайному режимі
	if (val > 0.1)
		val -= val*0.01;
	// При малій кількості енергії тіла енергія втрачається в меншому розмірі(спрощення розрахунків механіки руху)
	else
		val -= val*0.008;

	// Якщо кількість енергії вже незначна ми прирівнюємо її до нуля, для пришвидшення ігрового процесу
	if (val < 0.001)
		set_value(0);
	// інакше ми записуємо значення що ми вирахували в функції
	else
		set_value(val);
}

Energy::~Energy(){}