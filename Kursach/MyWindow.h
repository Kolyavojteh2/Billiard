#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Menu.h"
#include "Game.h"

using namespace sf;
using namespace std;

class MyWindow
{
	RenderWindow* window;

	int state;			// �������� ���� ��������

	Menu* menu;			// ��������� �� ������� ����
	Menu* additional_window;		// ��������� �� ������� ���� � �����������
	Game* game;			// ��������� �� ���

	Music fone;			// ������ ������

	Image* icon;		// ������ ���

	// ��������� �� ��������� ��������� ����
	void create_main_menu();
	void delete_main_menu();

	// ��������� �� ��������� ���
	void create_game();
	void delete_game();

	// ��������� �� ��������� �������������� ���� ��� ���
	void create_about();
	void delete_about();

	// ��������� �� ��������� ���������� ���
	void create_manual();
	void delete_manual();

	// ��������� �� ��������� ������� ��������
	void create_intro();
	void delete_intro();

	// ��������� �� ��������� �������������� ���� ��� ����� ���
	void create_end_game();
	void delete_end_game();

public:
	MyWindow(VideoMode mode, string name);

	RenderWindow* get_window() const;
	// ������ ��������� ����� ����
	void event_handling();

	// ���� ���������� ������
	SoundBuffer click_buf;
	Sound s_click;

	~MyWindow();
};
