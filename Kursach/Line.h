#pragma once

#include "Point2D.h"

// ��������� �� ������� � ������� ������ ����, ��� �������� �������� � ���������� SFML �� std
namespace Vojtekh
{
	struct Line
	{
		// ����� ������� �� ���� ��
		point2D start, end;

		// ������� ��������, ������� ��, � ��� ������� ������ "����������" ���糿
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