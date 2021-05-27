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

	int state;			// Теперішній стан програми

	Menu* menu;			// указатель на головне меню
	Menu* additional_window;		// Указатель на внутрішнє вікно з інформацією
	Game* game;			// указатель на гру

	Music fone;			// фонова музика

	Image* icon;		// іконка гри

	// Створення та видалення головного меню
	void create_main_menu();
	void delete_main_menu();

	// Створення та видалення гри
	void create_game();
	void delete_game();

	// Створення та видалення інформаційного вікна про гру
	void create_about();
	void delete_about();

	// Створення та видалення інструкції гри
	void create_manual();
	void delete_manual();

	// Створення та видалення вступної заставки
	void create_intro();
	void delete_intro();

	// Створення та видалення інформаційного вікна про кінець гри
	void create_end_game();
	void delete_end_game();

public:
	MyWindow(VideoMode mode, string name);

	RenderWindow* get_window() const;
	// Запуск основного циклу вікна
	void event_handling();

	// Звук натискання кнопки
	SoundBuffer click_buf;
	Sound s_click;

	~MyWindow();
};
