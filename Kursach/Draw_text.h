#pragma once

#include <SFML/Graphics.hpp>
#include <string>

using namespace std;
using namespace sf;

// Клас для малювання тексту у вікні
class Draw_text
{
	Font font;		// шрифт тексту
	Text text;		// текст

	RenderWindow* window;	// вікно для малювання

public:
	Draw_text(RenderWindow* w);
	
	// Задання параметрів шрифта
	void set_text(string ss);
	void set_color(sf::Color c);
	void set_size(int n);

	// Повернення тексту
	string get_text() const;
	// Повернення кількості букв у рядку (необхідне для центрування тексту в кнопках)
	int get_size() const;

	void draw();

	~Draw_text();

	// Задання позиції
	void set_position(int x, int y);
};