#pragma once

#include "Draw_text.h"
#include "Draw_rect.h"

#include "Rectangle_shape.h"

// ���� ������ ��� �������� � ������ ����� �������� � �����
class Button : private Rectangle_shape, public Draw_text
{
	Texture* texture;

	// ���� � ���� ��������� ������� �� ��������� �� ������
	int jump_to_state;		
public:
	Button(RenderWindow* w);

	void set_position(int x, int y);
	void set_position(point2D p);
	point2D get_position() const;

	void set_texture(Texture* t);

	int get_height() const;
	int get_width() const;

	// ������� �� ���������� ���������� ��� ���� � ���� ��������� ������� �� ���������� �� ������
	int get_jump() const;
	void set_jump(int n);

	void draw();

	bool is_collision(float x, float y);

	~Button();
};