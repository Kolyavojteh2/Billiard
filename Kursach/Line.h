#pragma once

#include "Point2D.h"

// Структура лінії схована в окремий простір імен, щоб уникнути путанину з бібліотеками SFML та std
namespace Vojtekh
{
	struct Line
	{
		// Точки початку та кінця лінії
		point2D start, end;

		// Визначає напрямок, відносно лінії, з якої сторони будуть "прийматись" колізії
		enum direct
		{
			RIGHT = 0,
			RIGHT_UP,
			UP,
			LEFT_UP,
			LEFT,
			LEFT_DOWN,
			DOWN,
			RIGHT_DOWN
		} dir;

		Line() : start(), end() {}
	};
}