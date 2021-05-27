#pragma once

#include "Point2D.h"
#include "Draw_rect.h"
#include "Ball_shape.h"
#include "Game.h"
#include <vector>

//  лас вибору м€ча, €кий в≥дпов≥даЇ за позначенн€ вибраного м€ча та управл≥нн€ ним
class Select_ball
{
	Ball_shape* s;				// указатель на вибраний м€ча
	Draw_rect* cirkle;			// коло вибора м€ча
	Texture* texture_cirkle;	// текстура кола
	RenderWindow* window;		// в≥кно дл€ малюванн€

	bool block;					// прапорець дл€ блокуванн€ вибору м€ч≥в

	Clock timer;				// таймер затримки м≥ж под≥€ми
	int delay;					// затримка

public:
	Select_ball(RenderWindow* w);

	// ƒоступ до вибраного шару
	Ball_shape* get_Ball() const;

	// ќсновн≥ функц≥њ ≥грового циклу
	void event_obj(bool is_end_move);
	void update(std::vector<Ball_shape*> arr, bool is_end_move);
	void draw();

	// ѕеренесенн€ спрайта за меж≥ екрану
	void remove_position();

	//  лас гра об'€вл€Їтьс€ €к дружн≥й дл€ спрощенн€ доступу до закритих член≥в класу вибору м'€ча
	friend class Game;

	~Select_ball();
};