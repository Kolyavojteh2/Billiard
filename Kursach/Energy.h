#pragma once

#include "Vector_rad.h"

// ���� ����㳿 ���������� �� ������� � �� ������� ��� ������ � ��������
class Energy : public Vojtekh::Vector_rad
{
public:
	Energy();
	Energy(float e, float a = 0);

	float get_speed() const;
	void set_speed(float n);
	
	// ������� ��������� ��� ��������� ����㳿 � ������ �������� ����
	void update(float time);

	~Energy();
};
