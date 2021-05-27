#pragma once

#include "Vector_rad.h"

// Клас енергії наслідується від вектора і має функції для роботи з швидкістю
class Energy : public Vojtekh::Vector_rad
{
public:
	Energy();
	Energy(float e, float a = 0);

	float get_speed() const;
	void set_speed(float n);
	
	// Функція оновлення для зменшення енергії з кожним проміжком часу
	void update(float time);

	~Energy();
};
