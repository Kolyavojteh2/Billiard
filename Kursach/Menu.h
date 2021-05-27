#pragma once

#include <vector>
#include "Draw_text.h"
#include "Button.h"
#include <SFML/Audio.hpp>

using namespace std;

// ���� �� ����� ���������� ������
extern const char* dir_ball_s;

class Menu
{
	vector<Button* > buttons;		// ����� ������
	vector<Draw_text* > textes;		// ����� ������

	Draw_rect* background;			// ������ ��������
	Texture* t_background;			// �������� ������ ��������

	RenderWindow* window;
	int this_state;					// �������� ���� ����� ����

	Sound* s_click;					// ��������� �� ���� ���������� ������
public:
	// ����������� ���� ��������� �� ���� ������ �� � ��������� �� ���� ���������� ������
	Menu(RenderWindow* w, Sound* s);

	void add_Button(Button* b);			// ��������� ������ �� ������
	void add_Draw_text(Draw_text* t);	// ��������� ������ �� ������

	void set_background(Texture* t);	// ������������ �������� ����
	
	// ��������� �� ������������ ����������� ����� ����
	int get_jump_state() const;
	void set_jump_state(int n);

	int events_obj();
	void draw();

	~Menu();
};