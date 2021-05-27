#pragma once

#include <vector>
#include "Ball_shape.h"
#include "Rectangle_collision.h"
#include "Select_Ball.h"
#include "Line.h"
#include "Draw_rect.h"
#include "Rectangle_shape.h"
#include "Draw_text.h"
#include <SFML/Audio.hpp>

using namespace std;

class Select_ball;

class Game
{
	// Масиви м'ячів, бордюрів та лунок стола
	vector<Ball_shape*> balls;
	vector<Vojtekh::Line*> borders;
	vector<Ball_collision*> holes;

	// Масив текстур
	vector<Texture*> textures;

	// Вказівники вибору м'яча, спрайта стола та більярдного кия
	Select_ball* selected;
	Draw_rect* board;
	Draw_rect* billiard_cue;

	// Вказівники шкали сили та його покажчика
	Rectangle_shape* power_bar;
	Draw_rect* select_rect;

	// Інформація про кількість ходів та неправильний удар
	Draw_text* info_moves;
	Draw_text* info_error;
	bool draw_error;

	RenderWindow* window;

	// Колізії м'ячів з різноманітними об'єктами
	bool collision_ball_ball(Ball_shape* a, Ball_shape* b);
	bool collision_ball_line(Ball_shape* a, Vojtekh::Line* b);
	bool collision_ball_hole(Ball_shape* a, Ball_collision* b);

	// Коректування наскочення м'яча на бордюр
	bool correct_balls_on_line(Ball_shape* a, Vojtekh::Line* b);

	// Ініціалізація лунок та бордюрів
	void init_borders();
	void init_holes();

	// Значення енергії вибраної на шкалі
	float power_value;
	
	// Лічильники ходів, промахів та неправильних ударів
	int move_number;
	int fines_number;
	int failures_number;

	// Буфера звуків: неправильного удару, колізії м'яч-м'яч, колізії кий-м'яч, колізії м'яч-бордюр, потрапляння м'яча в лунку
	SoundBuffer error_buf;
	SoundBuffer balls_buf;
	SoundBuffer cue_buf;
	SoundBuffer border_buf;
	SoundBuffer hole_buf;

	// Звуки: аналогічно буферам
	Sound s_error;
	Sound s_balls;
	Sound s_cue;
	Sound s_border;
	Sound s_hole;

	// Прапорці
	bool is_set_power;				// вибір сили на шкалі
	bool show_select_rect;			// малювання вибраного м'яча
	bool is_end_move;				// кінець ходу
	bool is_collision_balls;		// колізія м'ячів за хід
	bool is_set_penalty;			// неправильний удар
	bool increment_move_number;		// чи можна збільшувати кількість ходів
	bool is_end_game;				// кінець гри

	point2D old_pos;				// стара позиція м'яча при потраплянні його в лунку без колізії з іншими м'ячами

	int this_state;					// внутрішній стан гри
public:
	Game(RenderWindow* w);

	// Запуск та ініціалізація гри
	void start();

	// Основні функції ігрового циклу
	int event_obj();
	void update(float time);
	void draw();

	// Доступ до інформації про закінчення гри
	bool end_game() const;

	// Доступ до лічильників
	int get_moves() const;
	int get_fines() const;
	int get_failures() const;

	~Game();

	// Кількість м'ячів на ігровому столі
	const int MAX_NUMBER_OF_BALLS = 16;
	
};