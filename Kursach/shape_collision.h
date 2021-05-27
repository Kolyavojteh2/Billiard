#pragma once

// Абстрактний клас для колізій мяча та прямокутника з чисто віртуальною функцією яка приймає координату точки.
// Клас є інтерфейсом для похідних класів.
class shape_collision
{
public:
	virtual bool is_collision(float x, float y) = 0;
};
