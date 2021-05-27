#pragma once

#include <vector>
#include "Draw_text.h"
#include "Button.h"
#include <SFML/Audio.hpp>

using namespace std;

// Шлях до звуку натискання кнопки
extern const char* dir_ball_s;

class Menu
{
	vector<Button* > buttons;		// Масив кнопок
	vector<Draw_text* > textes;		// Масив текстів

	Draw_rect* background;			// Фонова картинка
	Texture* t_background;			// Текстура фонової картинки

	RenderWindow* window;
	int this_state;					// Внутрішній стан цього меню

	Sound* s_click;					// Указатель на звук натискання кнопки
public:
	// Конструктор окрім указателя на вікно приймає ще і указатель на звук натискання кнопки
	Menu(RenderWindow* w, Sound* s);

	void add_Button(Button* b);			// Додавання кнопки до масиву
	void add_Draw_text(Draw_text* t);	// Додавання тексту до масиву

	void set_background(Texture* t);	// Встановлення текстури фону
	
	// Поверення та встановлення внутрішнього стану меню
	int get_jump_state() const;
	void set_jump_state(int n);

	int events_obj();
	void draw();

	~Menu();
};