#pragma once

// ����������� ���� ��� ����� ���� �� ������������ � ����� ���������� �������� ��� ������ ���������� �����.
// ���� � ����������� ��� �������� �����.
class shape_collision
{
public:
	virtual bool is_collision(float x, float y) = 0;
};
