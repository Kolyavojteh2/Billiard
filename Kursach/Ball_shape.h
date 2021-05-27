#pragma once

#include "Energy.h"
#include "Ball_collision.h"
#include "Draw_rect.h"

// ���� ������ �'��� ���� ��'���� ���� ��� ��������� �� ���糿 �'���
class Ball_shape : public Draw_rect, public Ball_collision
{
private:
	Energy energy;	// ������ ���

	bool is_select;	// �� ������� �'��
	bool is_set_direct_;	// �� ������� �������� ����� ��� �� �'���

	int delay;		// �������� ��� �������� ������ �'��� �� ������ ��������
	Clock timer;	// ������ ��� ��������

	float dx, dy;	// ������� �'��� �� ���� ���� �� ���� � �� �
public:
	Ball_shape();
	Ball_shape(RenderWindow* w);

	// ���������� ����㳿 �� ����
	float get_energy() const;
	float get_angle_energy() const;

	// ������� ����㳿 �� ����
	void set_energy(float n);
	void set_angle_energy(float n);

	// ���� �� ������ ������ ����� ����
	void select_ball(bool b = true);
	void unselect_ball();

	// ������� ��������
	void set_texture(Texture* t);
	
	// ������� ������� �� �� ���������� 
	void set_position_shape(float x, float y);
	point2D get_position_shape() const;

	// ���������� �������� ���������
	bool is_set_direct() const;
	bool is_selected() const;

	// ������ ������� ��������� ����� ���
	void events_obj();
	void draw();
	void update(float time);

	~Ball_shape();
};