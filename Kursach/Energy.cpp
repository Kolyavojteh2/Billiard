#include "Energy.h"
#include <cmath>

#include <iostream>

using namespace Vojtekh;

Energy::Energy() : Vector_rad() {}
Energy::Energy(float e, float a) : Vector_rad(a, e) {}

// ���������� �������� ��'����(���� ������ ������� ������ ��'����)
float Energy::get_speed() const { return sqrt(get_value()); }

// ������� �������� ��'����
void Energy::set_speed(float n) { set_value(n*n); }

// ��������� �������� ����㳿
// ������� ����� ��� �� ��������
void Energy::update(float time)
{
	float val = get_value();

	// ���� ������ ������� �� ������ �������� � �������
	if (val == 0)
		return;

	// ��� ������ ������� ����㳿 ��� ������ ���������� � ���������� �����
	if (val > 0.1)
		val -= val*0.01;
	// ��� ���� ������� ����㳿 ��� ������ ���������� � ������� �����(��������� ���������� ������� ����)
	else
		val -= val*0.008;

	// ���� ������� ����㳿 ��� �������� �� ���������� �� �� ����, ��� ������������ �������� �������
	if (val < 0.001)
		set_value(0);
	// ������ �� �������� �������� �� �� ���������� � �������
	else
		set_value(val);
}

Energy::~Energy(){}