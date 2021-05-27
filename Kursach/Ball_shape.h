#pragma once

#include "Energy.h"
#include "Ball_collision.h"
#include "Draw_rect.h"

// Клас фігури м'яча який об'єднує клас для малювання та колізії м'яча
class Ball_shape : public Draw_rect, public Ball_collision
{
private:
	Energy energy;	// енергія тіла

	bool is_select;	// чи вибрано м'яч
	bool is_set_direct_;	// чи вибрано напрямок удару києм по м'ячу

	int delay;		// затримка для операцій вибору м'яча та вибору напрямку
	Clock timer;	// таймер для затримки

	float dx, dy;	// зміщення м'яча за один такт по осям х та у
public:
	Ball_shape();
	Ball_shape(RenderWindow* w);

	// Повернення енергії та кута
	float get_energy() const;
	float get_angle_energy() const;

	// Задання енергії та кута
	void set_energy(float n);
	void set_angle_energy(float n);

	// Вибір та зняття вибору цього шару
	void select_ball(bool b = true);
	void unselect_ball();

	// Задання текстури
	void set_texture(Texture* t);
	
	// Задання позиції та її повернення 
	void set_position_shape(float x, float y);
	point2D get_position_shape() const;

	// Повернення значення прапорців
	bool is_set_direct() const;
	bool is_selected() const;

	// Основні функції головного циклу гри
	void events_obj();
	void draw();
	void update(float time);

	~Ball_shape();
};