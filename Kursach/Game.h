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
	// ������ �'����, ������� �� ����� �����
	vector<Ball_shape*> balls;
	vector<Vojtekh::Line*> borders;
	vector<Ball_collision*> holes;

	// ����� �������
	vector<Texture*> textures;

	// ��������� ������ �'���, ������� ����� �� ���������� ���
	Select_ball* selected;
	Draw_rect* board;
	Draw_rect* billiard_cue;

	// ��������� ����� ���� �� ���� ���������
	Rectangle_shape* power_bar;
	Draw_rect* select_rect;

	// ���������� ��� ������� ���� �� ������������ ����
	Draw_text* info_moves;
	Draw_text* info_error;
	bool draw_error;

	RenderWindow* window;

	// ���糿 �'���� � ������������ ��'������
	bool collision_ball_ball(Ball_shape* a, Ball_shape* b);
	bool collision_ball_line(Ball_shape* a, Vojtekh::Line* b);
	bool collision_ball_hole(Ball_shape* a, Ball_collision* b);

	// ������������ ���������� �'��� �� ������
	bool correct_balls_on_line(Ball_shape* a, Vojtekh::Line* b);

	// ����������� ����� �� �������
	void init_borders();
	void init_holes();

	// �������� ����㳿 ������� �� ����
	float power_value;
	
	// ˳�������� ����, �������� �� ������������ �����
	int move_number;
	int fines_number;
	int failures_number;

	// ������ �����: ������������� �����, ���糿 �'��-�'��, ���糿 ���-�'��, ���糿 �'��-������, ����������� �'��� � �����
	SoundBuffer error_buf;
	SoundBuffer balls_buf;
	SoundBuffer cue_buf;
	SoundBuffer border_buf;
	SoundBuffer hole_buf;

	// �����: ��������� �������
	Sound s_error;
	Sound s_balls;
	Sound s_cue;
	Sound s_border;
	Sound s_hole;

	// ��������
	bool is_set_power;				// ���� ���� �� ����
	bool show_select_rect;			// ��������� ��������� �'���
	bool is_end_move;				// ����� ����
	bool is_collision_balls;		// ����� �'���� �� ���
	bool is_set_penalty;			// ������������ ����
	bool increment_move_number;		// �� ����� ���������� ������� ����
	bool is_end_game;				// ����� ���

	point2D old_pos;				// ����� ������� �'��� ��� ���������� ���� � ����� ��� ���糿 � ������ �'�����

	int this_state;					// �������� ���� ���
public:
	Game(RenderWindow* w);

	// ������ �� ����������� ���
	void start();

	// ������ ������� �������� �����
	int event_obj();
	void update(float time);
	void draw();

	// ������ �� ���������� ��� ��������� ���
	bool end_game() const;

	// ������ �� ���������
	int get_moves() const;
	int get_fines() const;
	int get_failures() const;

	~Game();

	// ʳ������ �'���� �� �������� ����
	const int MAX_NUMBER_OF_BALLS = 16;
	
};