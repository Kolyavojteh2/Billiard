#pragma once

#include "Point2D.h"
#include "Draw_rect.h"
#include "Ball_shape.h"
#include "Game.h"
#include <vector>

// ���� ������ ����, ���� ������� �� ���������� ��������� ���� �� ��������� ���
class Select_ball
{
	Ball_shape* s;				// ��������� �� �������� ����
	Draw_rect* cirkle;			// ���� ������ ����
	Texture* texture_cirkle;	// �������� ����
	RenderWindow* window;		// ���� ��� ���������

	bool block;					// ��������� ��� ���������� ������ �����

	Clock timer;				// ������ �������� �� ������
	int delay;					// ��������

public:
	Select_ball(RenderWindow* w);

	// ������ �� ��������� ����
	Ball_shape* get_Ball() const;

	// ������ ������� �������� �����
	void event_obj(bool is_end_move);
	void update(std::vector<Ball_shape*> arr, bool is_end_move);
	void draw();

	// ����������� ������� �� ��� ������
	void remove_position();

	// ���� ��� ��'��������� �� ������ ��� ��������� ������� �� �������� ����� ����� ������ �'���
	friend class Game;

	~Select_ball();
};