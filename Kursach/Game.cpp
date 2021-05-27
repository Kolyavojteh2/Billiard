#include "Game.h"
#include "Vector_rad.h"
#include "Equation.h"
#include <cmath>

#include <iostream>

#include "convert_string.h"

using namespace std;

// ���������
const float PI = 3.14159265;
const int SIZE_OF_BALL = 16;
const float COS_OR_SIN_45 = 0.707106;

const float MIN_POWER = 0;
const float MAX_POWER = 1;

// ������ ������� �� �����
extern const char* dir_error_s;
extern const char* dir_ball_s;
extern const char* dir_border_s;
extern const char* dir_cue_s;
extern const char* dir_hole_s;

extern const char* dir_ball;
extern const char* dir_cue;
extern const char* dir_select;
extern const char* dir_power;
extern const char* dir_board;

// ����������� ���������� ������� �� ����������� �����
Game::Game(RenderWindow* w) : window(w), board(nullptr), 
	is_end_move(true), move_number(0), is_end_game(false), this_state(3),
	draw_error(false)
{
	error_buf.loadFromFile(dir_error_s);
	balls_buf.loadFromFile(dir_ball_s);
	cue_buf.loadFromFile(dir_cue_s);
	border_buf.loadFromFile(dir_border_s);
	hole_buf.loadFromFile(dir_hole_s);

	s_balls.setBuffer(balls_buf);
	s_border.setBuffer(border_buf);
	s_cue.setBuffer(cue_buf);
	s_error.setBuffer(error_buf);
	s_hole.setBuffer(hole_buf);
}

// ��������� ��� ��'���� ���
Game::~Game()
{
	// ��������� �������
	for (auto it = textures.begin(); it != textures.end(); it++)
	{
		if (*it)
			delete *it;
	}

	// ��������� �������� �����
	if (board)
		delete board;

	// ��������� �'����, �� ���������� �� ����
	for (auto it = balls.begin(); it != balls.end(); it++)
	{
		if (*it)
			delete *it;
	}

	// ��������� ����� ����� �����
	for (auto it = borders.begin(); it != borders.end(); it++)
	{
		if (*it)
			delete *it;
	}

	// ��������� ����� �����
	for (auto it = holes.begin(); it != holes.end(); it++)
		if (*it)
			delete *it;

	// ��������� ��'���� ������ �'���
	if (selected)
		delete selected;

	// ��������� �������� ������ ���� �����
	if (power_bar)
		delete power_bar;

	// ��������� �������� ������� ������ ���� �����
	if (select_rect)
		delete select_rect;

	// ��������� �������� ���
	if (billiard_cue)
		delete billiard_cue;

	// ��������� ������ ��� ������� ����
	if (info_error)
		delete info_error;

	// ��������� ������ ��� ������������ ����
	if (info_moves)
		delete info_moves;
}

// ��������� ��'���� ���
void Game::start()
{
// ����
	Texture* texture_board = new Texture;
	texture_board->loadFromFile(dir_board);
	textures.push_back(texture_board);

	board = new Draw_rect();
	board->set_window(window);
	board->set_texture(texture_board);
	board->set_position_draw_rect(window->getSize().x - 1200, window->getSize().y - 670);

// ������ ����
	Texture* texture_power = new Texture;
	texture_power->loadFromFile(dir_power);
	textures.push_back(texture_power);

	power_bar = new Rectangle_shape(window);
	power_bar->set_texture(texture_power);
	power_bar->set_position_shape(board->get_position_draw_rect().x - 100, board->get_position_draw_rect().y);

// ������ ������ ����
	Texture* t_select_rect = new Texture;
	t_select_rect->loadFromFile(dir_select);
	textures.push_back(t_select_rect);

	select_rect = new Draw_rect(window);
	select_rect->set_texture(t_select_rect);
	select_rect->set_position_draw_rect(board->get_position_draw_rect().x - 100, board->get_position_draw_rect().y);
	show_select_rect = false;

// ���
	Texture* t_cue = new Texture;
	t_cue->loadFromFile(dir_cue);
	textures.push_back(t_cue);

	billiard_cue = new Draw_rect(window);
	billiard_cue->set_texture(t_cue);
	billiard_cue->set_position_draw_rect(-1000, -1000);

// �'���
	Texture* texture_ball = new Texture;
	texture_ball->loadFromFile(dir_ball);
	textures.push_back(texture_ball);

	for (int i = 0; i < MAX_NUMBER_OF_BALLS; i++)
	{
		Ball_shape* b = new Ball_shape(window);
		b->set_texture(texture_ball);

		balls.push_back(b);
	}
	// ��������� �'���� �������� �������
	int h = 0;
	// ��������� �'����
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < i + 1; j++)
		{
			balls.at(h)->set_position_shape(board->get_position_draw_rect().x + 800 + (28 + 1) * i, board->get_position_draw_rect().y + 335 + (16 + 1) * i - (32 + 1) * j);
			h++;

			if (h >= MAX_NUMBER_OF_BALLS)
				break;
		}
		if (h >= MAX_NUMBER_OF_BALLS)
			break;
	}

	// �����
	if (h == MAX_NUMBER_OF_BALLS - 1)
	{
		balls.at(h)->set_position_shape(board->get_position_draw_rect().x + 300, 335 + board->get_position_draw_rect().y);
	}

// �������
	init_borders();
// �����
	init_holes();
// �������� ���
	selected = new Select_ball(window);

// ���������� ��� ����� ���� �� �������
	info_moves = new Draw_text(window);
	info_moves->set_color(sf::Color::Black);
	info_moves->set_size(40);
	string ss = "Stroke number: ";
	ss += Vojtekh::convert_string::to_string<int>(move_number);
	info_moves->set_text(ss);
	info_moves->set_position(100, 50);

	info_error = new Draw_text(window);
	info_error->set_color(sf::Color::Red);
	info_error->set_size(40);
	info_error->set_text("The ball did not touch the other balls!!!");
	info_error->set_position(650, 50);
}

// ������� ������� ���� ���
int Game::event_obj()
{
	// ������� ���� �'����
	for (auto it = balls.begin(); it != balls.end(); it++)
	{
		if(*it)
			(*it)->events_obj();
	}

	// ������� ���� ������ ����
	selected->event_obj(is_end_move);

	// ������� ���� ����� ���� �����
	power_bar->events_obj();

	// ���������� ��������� ������� ������ �� ���� ���� �����
	auto abs_pos_mouse = Mouse::getPosition();
	// ���������� ��������� ���� � ����
	point2D pos_mouse_on_rect;
	pos_mouse_on_rect.x = abs_pos_mouse.x - window->getPosition().x - power_bar->get_position_shape().x - 8;
	pos_mouse_on_rect.y = abs_pos_mouse.y - window->getPosition().y - power_bar->get_position_shape().y - 40;
	
	// ������� ��䳿 ���������� ��� � ���� ���� �����
	if (Mouse::isButtonPressed(Mouse::Button::Left) && 
		(pos_mouse_on_rect.x >= 0 && pos_mouse_on_rect.x < power_bar->Rectangle_collision::get_width()) && 
		(pos_mouse_on_rect.y >= 0 && pos_mouse_on_rect.y < power_bar->Rectangle_collision::get_height()))
	{
		// �������� �� �������� �'��
		if (selected->get_Ball())
		{
			// �������� �� �������� �'���
			if (selected->get_Ball()->get_size() == 16)
			{
				// ���������� �� ������� ��������� ������ ������ ����
				float power = (1 - pos_mouse_on_rect.y / abs(power_bar->Rectangle_collision::get_height() - 0)) * abs(MIN_POWER - MAX_POWER);
				power_value = power;
				is_set_power = true;
				show_select_rect = true;
				select_rect->set_position_draw_rect(select_rect->get_position_draw_rect().x, pos_mouse_on_rect.y + power_bar->get_position_shape().y + 2);
			}
		}
	}

	// ������� ��䳿 ����� ��� �� �'���
	if (Keyboard::isKeyPressed(Keyboard::Key::Space) && is_set_power)
	{
		// �������� ������� ������ ����
		is_set_power = false;

		// �������� �� �'�� �� �� �������� ��������
		if (selected->get_Ball())
		{
			// �������� �� �������� ��'����
			if (selected->get_Ball()->get_size() == 16)
			{
				// ��������� �'��� ���� ����㳿
				selected->get_Ball()->set_energy(power_value);

				// ������ ������ �� �����
				selected->get_Ball()->unselect_ball();
				selected->block = false;

				// �����'���������� ����� ������� �'��� ���� �'�� ��� ���������� �� ����� �'���� ��������� � �����
				old_pos = selected->get_Ball()->get_position_shape();

				// ��������� ��'���� ��������� ���� ��������� ���������
				selected->s = nullptr;
				selected->remove_position();

				// �������� ���������� ��� �� ��� ������ ���������
				billiard_cue->set_position_draw_rect(-1000, -1000);

				// ����������� ����� ����� ��� �� �'���
				s_cue.play();
			}
		}

		// �������� �������� ��������� ��� ����� �'���
		show_select_rect = false;
	}

	// ������� ��䳿 ���� ������ ����� ���
	if (is_end_game)
		return 7;

	// ���� ��������� ESCAPE �� �������� � ������� ����
	if(Keyboard::isKeyPressed(Keyboard::Key::Escape))
		return 2;

	// ���������� ����������� ����� ���
	return this_state;
}

// ��������� ����� ���
void Game::update(float time)
{
	// ��������� ������ ��� ����� ����
	string ss = "Stroke number: ";
	ss += Vojtekh::convert_string::to_string<int>(move_number);
	info_moves->set_text(ss);

	// �������� �� ����� ���
	if (balls.size() < 2)
		is_end_game = true;

	// ������� ������ ����������
	for (auto it = balls.begin(); it != balls.end(); it++)
		if (!(*it))
			balls.erase(it);

	// ���� ����� �'��� ����� �� �������� �������� ������ ���
	if (!selected->get_Ball())
		billiard_cue->set_position_draw_rect(-1000, -1000);

	// �������� �� ����� ���� � ��������� � ��������� �������� ��������, ����, �������
	if (increment_move_number && is_end_move)
	{
		if (is_collision_balls) 
			failures_number++;
		else
			is_set_penalty = true;

		if (is_set_penalty)
		{
			fines_number++;
			s_error.play();
			draw_error = true;
		}
		else
			draw_error = false;

		move_number++;
	}
		
	
	// �������� �������� �� ������� ����� ���� �� ���
	if (is_end_move)
	{
		increment_move_number = false;
		is_set_penalty = false;
		is_collision_balls = false;
	}
	else
		increment_move_number = true;

	// ��� ������� �����
	for (auto it = balls.begin(); it != balls.end(); it++)
	{
		// ���糿 ���-���
		for (auto jt = it + 1; jt != balls.end(); jt++)
		{
			bool b = collision_ball_ball((*it), (*jt));
			if (b)
			{
				// ������������ �������� ���������� �'���� �� ���� ���
				is_collision_balls = b;
				// ������ ����� ���������� �'���� �� �����
				s_balls.play();

				// ����������� ���� � ���������� ����� ��� ��������� ���� �� ������ �� ����
				if ((*it)->get_energy() == 0.0)
					(*it)->set_energy(0.01);

			}
		}

		// ���糿 ���-������
		for (auto jt = borders.begin(); jt != borders.end(); jt++)
		{
			if (collision_ball_line((*it), (*jt)))
			{
				// ������ ����� ���������� �'��� �� �������
				s_border.play();
				// ������������ ������� �'��� ������� �������
				correct_balls_on_line((*it), (*jt));
				break;
			}
		}

		// ���糿 ���-�����
		for (auto jt = holes.begin(); jt != holes.end(); jt++)
		{
			if (collision_ball_hole(*it, *jt))
			{
				// ������ ����� ������ �'��� � �����
				s_hole.play();

				// ���� �� ������� ��������� �'���� �� ����� � �'�� ���� � ����� ��������� ���� �� �������� ����
				if (!is_collision_balls)
				{
					(*it)->set_position_shape(old_pos.x, old_pos.y);
					(*it)->set_energy(0);
					// ������ ����� ��� ������������ ����
					s_error.play();
				}
				// ���� ���� ��� ���������� ��������� �'��
				else
				{
					auto t = *it;

					it = balls.erase(it);
					delete t;

					failures_number--;

					// ����� � ����� ��� ��������� ���� ������ �'����(�� ����� ��� �������� ������ �� ��� ������)
					if (it == balls.end())
						break;
				}
			}
		}
		// ����� � ����� ��� �������� ���� ������ �'����
		if (it == balls.end())
			break;

	}

	// ��������� ����� �'����
	for (auto it = balls.begin(); it != balls.end(); it++)
	{
		if (*it)
			(*it)->update(time);
	}

	// �������� �� ����� ����(���������� ����)
	bool temp = true;
	for (auto it = balls.begin(); it != balls.end(); it++)
	{
		if ((*it)->get_energy() != 0)
		{
			is_end_move = false;
			temp = false;
			break;
		}
	}
	// ������������ �������� ���� ����
	if (temp)
		is_end_move = true;

	// ���� �� ������� ���糿 � ������� ���� ����� �� 16 �� ������� � �������� ����� ���
	if (increment_move_number && is_end_move && !is_collision_balls && balls.size() < MAX_NUMBER_OF_BALLS)
	{
		// ��������� ����
		bool ok = false;
		Ball_shape* sh = new Ball_shape(window);
		sh->set_texture(textures.at(4));

		// ������������ ������� ����
		while (!ok)
		{
			int dx, dy;
			srand(::time(NULL));

			// ���������� ��������� ��������� �� ����
			dx = board->get_position_draw_rect().x + 86 + rand() % 1027;
			dy = board->get_position_draw_rect().y + 86 + rand() % 498;

			sh->set_position_shape(dx, dy);

			// ���������� ����������� ������������ �'���(�� �� ����������� �� ������ �'���)
			bool b = false;
			for (auto it = balls.begin(); it != balls.end(); it++)
			{
				b = collision_ball_ball(sh, (*it));
				if (b)
				{
					ok = false;
					break;
				}
			}

			// ���� ��� ��������� �� ����� ��� �� ������� ������� ����
			if (!b)
				ok = true;
		}

		balls.push_back(sh);
	}

	// ��������� ����� ����� ����
	power_bar->update(time);

	// ��������� ����� ��'���� ������ �'���
	selected->update(balls, is_end_move);

	// ���������� ��������� �� ����� �������� ���������� ���
	if (selected->get_Ball() && selected->get_Ball()->get_size() == 16 && selected->get_Ball()->is_selected() && selected->block && !selected->get_Ball()->is_set_direct())
	{
		auto b = selected->get_Ball();

		// ���� ��������� ���� ������� ����, �� ���������� ��� ���� ����
		Vector2i pos_mouse = Mouse::getPosition();
		Vector2i pos_wind = window->getPosition();
		pos_mouse.x -= pos_wind.x;
		pos_mouse.y -= pos_wind.y;

		point2D pos_ball;
		pos_ball.x = b->get_pos_x() - pos_mouse.x;
		pos_ball.y = b->get_pos_y() + 16 + 24 - pos_mouse.y;

		float size_vec = sqrt(pos_ball.x*pos_ball.x + pos_ball.y*pos_ball.y);

		float cos_a = pos_ball.x / size_vec;
		float sin_a = pos_ball.y / size_vec;

		float a = Vojtekh::get_angle(sin_a, cos_a);

		float old = billiard_cue->get_rotate_angle();
		a = a * 180 / PI;
		a = a - old;
		a += 90;

		billiard_cue->set_rotate_angle(a);


		float angle = a * PI / 180;
		point2D sh;
		int pixel = 1;
		sh.x = sin(angle) * pixel;
		sh.y = cos(angle) * pixel;
		billiard_cue->set_position_draw_rect(selected->get_Ball()->get_pos_x() + sh.x, selected->get_Ball()->get_pos_y() + sh.y );
	}

	// FLAGS
	if (!selected->block)
		is_set_power = false;
}

// ������������ ������� �'��� ������� �������
bool Game::correct_balls_on_line(Ball_shape* a, Vojtekh::Line* b)
{
	// ���� �'�� �� �������� ��������, �� ��������
	if (!a)
		return false;

	// ���������� ������� ���糿 ������� �� ������������ ������� �'���
	switch (b->dir)
	{
	case Vojtekh::Line::direct::RIGHT:
	{
		while (a->is_collision(b->start.x, b->start.y) || a->is_collision(b->end.x, b->end.y))
		{
			auto pos = a->get_position_shape();
			pos.x++;
			a->set_position_shape(pos.x, pos.y);
		}
		auto pos = a->get_position_shape();
		pos.x++;
		a->set_position_shape(pos.x, pos.y);

		break;
	}

	case Vojtekh::Line::direct::LEFT:
	{
		while (a->is_collision(b->start.x, b->start.y) || a->is_collision(b->end.x, b->end.y))
		{
			auto pos = a->get_position_shape();
			pos.x--;
			a->set_position_shape(pos.x, pos.y);
		}
		auto pos = a->get_position_shape();
		pos.x--;
		a->set_position_shape(pos.x, pos.y);

		break;
	}

	case Vojtekh::Line::direct::DOWN:
	{
		while (a->is_collision(b->start.x, b->start.y) || a->is_collision(b->end.x, b->end.y))
		{
			auto pos = a->get_position_shape();
			pos.y++;
			a->set_position_shape(pos.x, pos.y);
		}
		auto pos = a->get_position_shape();
		pos.y++;
		a->set_position_shape(pos.x, pos.y);

		break;
	}
	case Vojtekh::Line::direct::UP:
	{
		while (a->is_collision(b->start.x, b->start.y) || a->is_collision(b->end.x, b->end.y))
		{
			auto pos = a->get_position_shape();
			pos.y--;
			a->set_position_shape(pos.x, pos.y);
		}

		auto pos = a->get_position_shape();
		pos.y--;
		a->set_position_shape(pos.x, pos.y);

		break;
	}

	default:
		break;
	}
	
	return true;
}

// ��������� ��'����
void Game::draw()
{
	// ������� ����
	window->clear(sf::Color::White);
	
	// ��������� �����
	board->draw();

	// ��������� �'����
	for (auto it = balls.begin(); it != balls.end(); it++)
	{	
		if (*it)
			(*it)->draw();
	}

	// ��������� ��������� �'���
	selected->draw();

	// ��������� ����� ������ ����
	power_bar->draw();

	// ��������� ��������� �'���
	if (show_select_rect)
		select_rect->draw();

	// ��������� ���������� ���
	if (selected->get_Ball() && selected->get_Ball()->get_size() == 16)
		billiard_cue->draw();

	// ��������� ������
	info_moves->draw();
	if (draw_error)
		info_error->draw();
}

// ������� ���������� ��������� �� �'���� �� ���(��������)
bool Game::collision_ball_line(Ball_shape* a, Vojtekh::Line* b)
{
	// ���� �'�� �� �������� ��������, �� ��������
	if (!a)
		return false;

	// ��������� ����� ����� ���糿 �� �'���
	point2D balls[4];
	for (int i = 0; i < 4; i++)
		balls[i] = a->get_position_shape();
	
	balls[0].x += SIZE_OF_BALL;
	balls[1].y += SIZE_OF_BALL;
	balls[2].x -= SIZE_OF_BALL;
	balls[3].y -= SIZE_OF_BALL;

	// ���������� �������� ���糿 �� �� ���������� ����� ������� ���� �� ��������
	switch (b->dir)
	{
	case Vojtekh::Line::direct::RIGHT:
	{
		int i = 2;
		{
			// �������� �� ���������� ��� �� ��� �� �� ������� �� �� ����������� � � �� ���
			if (b->start.x >= balls[i].x && (balls[i].y > b->start.y) && (balls[i].y < b->end.y))
			{
				// ���� ������� ���� �� ����㳿
				float angle_border = Vojtekh::get_angle_2_points(b->start, b->end);

				float new_angle = a->get_angle_energy() + (angle_border - a->get_angle_energy()) * 2;
				a->set_angle_energy(new_angle);
				a->set_energy(a->get_energy() * 0.9);

				// ���� ��� ����� ���� � �������� �� ���� ������ ���糿 �� �������� ���� �����
				if (b->start.x > a->get_position_shape().x - SIZE_OF_BALL)
				{
					float temp = a->get_position_shape().x - SIZE_OF_BALL - b->start.x;
					a->set_position_shape(a->get_position_shape().x - temp, a->get_position_shape().y);
				}

				return true;
			}
		}
		break;
	}

	case Vojtekh::Line::direct::LEFT:
	{
		int i = 0;
		{
			// �������� �� ���������� ��� �� ��� �� �� ������� �� �� ����������� � � �� ���
			if (b->start.x <= balls[i].x && (balls[i].y > b->start.y) && (balls[i].y < b->end.y))
			{
				// ���� ������� ���� �� ����㳿
				float angle_border = Vojtekh::get_angle_2_points(b->start, b->end);

				float new_angle = a->get_angle_energy() + (angle_border - a->get_angle_energy()) * 2;
				a->set_angle_energy(new_angle);
				a->set_energy(a->get_energy() * 0.9);

				// ���� ��� ����� ���� � �������� �� ���� ������ ���糿 �� �������� ���� �����
				if (b->start.x > a->get_position_shape().x + SIZE_OF_BALL)
				{
					float temp = a->get_position_shape().x + SIZE_OF_BALL - b->start.x;
					a->set_position_shape(a->get_position_shape().x + temp, a->get_position_shape().y);
				}

				return true;
			}
		}
		break;
	}

	case Vojtekh::Line::direct::DOWN:
	{
		int i = 3;
		{
			// �������� �� ���������� ��� �� ��� �� �� ������� �� �� ����������� � � �� ���
			if (b->start.y >= balls[i].y && (balls[i].x > b->start.x) && (balls[i].x < b->end.x))
			{
				// ���� ������� ���� �� ����㳿
				float angle_border = Vojtekh::get_angle_2_points(b->start, b->end);

				float new_angle = a->get_angle_energy() + (angle_border - a->get_angle_energy()) * 2;
				a->set_angle_energy(new_angle);
				a->set_energy(a->get_energy() * 0.9);

				// ���� ��� ����� ���� � �������� �� ���� ������ ���糿 �� �������� ���� �����
				if (b->start.y > a->get_position_shape().y + SIZE_OF_BALL)
				{
					float temp = a->get_position_shape().y + SIZE_OF_BALL - b->start.y;
					a->set_position_shape(a->get_position_shape().x, a->get_position_shape().y + temp);
				}

				return true;
			}
		}
		break;
	}

	case Vojtekh::Line::direct::UP:
	{
		int i = 1;
		{
			// �������� �� ���������� ��� �� ��� �� �� ������� �� �� ����������� � � �� ���
			if (b->start.y <= balls[i].y && (balls[i].x > b->start.x) && (balls[i].x < b->end.x))
			{
				// ���� ������� ���� �� ����㳿
				float angle_border = Vojtekh::get_angle_2_points(b->start, b->end);

				float new_angle = a->get_angle_energy() + (angle_border - a->get_angle_energy()) * 2;
				a->set_angle_energy(new_angle);
				a->set_energy(a->get_energy() * 0.9);

				// ���� ��� ����� ���� � �������� �� ���� ������ ���糿 �� �������� ���� �����
				if (b->start.y > a->get_position_shape().y + SIZE_OF_BALL)
				{
					float temp = a->get_position_shape().y + SIZE_OF_BALL - b->start.y;
					a->set_position_shape(a->get_position_shape().x, a->get_position_shape().y + temp);
				}

				return true;
			}
		}
		break;
	}

	case Vojtekh::Line::direct::RIGHT_UP:
	{
		if (Vojtekh::is_collision_ball_line(a, b))
		{
			// ���� ������� ���� �� ����㳿
			float angle_border = Vojtekh::get_angle_2_points(b->start, b->end);

			float new_angle = a->get_angle_energy() + (angle_border - a->get_angle_energy()) * 2;
			a->set_angle_energy(new_angle);
			a->set_energy(a->get_energy() * 0.9);
		}

		break;
	}
	case Vojtekh::Line::direct::RIGHT_DOWN:
	{
		if (Vojtekh::is_collision_ball_line(a, b))
		{
			// ���� ������� ���� �� ����㳿
			float angle_border = Vojtekh::get_angle_2_points(b->start, b->end);

			float new_angle = a->get_angle_energy() + (angle_border - a->get_angle_energy()) * 2;
			a->set_angle_energy(new_angle);
			a->set_energy(a->get_energy() * 0.9);
		}

		break;
	}
	case Vojtekh::Line::direct::LEFT_UP:
	{
		if (Vojtekh::is_collision_ball_line(a, b))
		{
			// ���� ������� ���� �� ����㳿
			float angle_border = Vojtekh::get_angle_2_points(b->start, b->end);

			float new_angle = a->get_angle_energy() + (angle_border - a->get_angle_energy()) * 2;
			a->set_angle_energy(new_angle);
			a->set_energy(a->get_energy() * 0.9);
		}

		break;
	}
	case Vojtekh::Line::direct::LEFT_DOWN:
	{
		if (Vojtekh::is_collision_ball_line(a, b))
		{
			// ���� ������� ���� �� ����㳿
			float angle_border = Vojtekh::get_angle_2_points(b->start, b->end);

			float new_angle = a->get_angle_energy() + (angle_border - a->get_angle_energy()) * 2;
			a->set_angle_energy(new_angle);
			a->set_energy(a->get_energy() * 0.9);
		}

		break;
	}

	}

	return false;
}

// ������� ���������� ��������� �� ����� �� ������
bool Game::collision_ball_hole(Ball_shape* a, Ball_collision* b)
{
	// ���� �'�� �� �������� ��������, �� ��������
	if (!a)
		return false;

	// ���� ���������� �'��� ����������� �������� �����, ��������� ��������� ���������
	if (b->is_collision(a->get_position_shape().x, a->get_position_shape().y))
	{
		return true;
	}
	
	// ������ ��������� ��������
	return false;
}

// ������� ���������� ��������� �� �'�����
bool Game::collision_ball_ball(Ball_shape* a, Ball_shape* b)
{
	// ���������� ���糿
	point2D ball_a, ball_b, ball_b1;

	ball_a = a->get_position_shape();
	ball_b = b->get_position_shape();

	ball_b1.x = ball_b.x - ball_a.x;
	ball_b1.y = ball_b.y - ball_a.y;

	float length_vector = sqrt(ball_b1.x*ball_b1.x + ball_b1.y*ball_b1.y);

	if (length_vector > a->get_size() * 2)
	{
		// ���糿 ����
		return false;
	}

	// ���������� ���� ������� �� ������
	float f1, f11, f21;

	f1 = a->get_angle_energy();
	f21 = Vojtekh::get_angle_2_points(ball_a, ball_b);

	// ���������� ������� ������� �������
	float t1, t2, t;
	t1 = f21 + PI / 2;
	t2 = f21 - PI / 2;

	if (abs(f1 - t1) <= (PI / 2))
		t = t1;
	else
		t = t2;
	// ���������� ������ ������� ���� 1
	f11 = f1 - (f1 - t) * 2;
	//a->set_angle_energy(f11);

	// ���������� ���� ���� ���� 1

	float value_a = a->get_energy();
	float angle_old_new;
	angle_old_new = f1 - f11;

	float value_f11 = abs(value_a*sin(angle_old_new));
	//a->set_energy(value_f11);

	// ��������� ������� 1 ���� �� ����� �����
	Vojtekh::Vector_rad F11;
	F11.set_angle(f11);
	F11.set_value(value_f11);

	// ��������� ������� 2 ���� �� ����� 1 ����
	Vojtekh::Vector_rad F21;
	F21.set_angle(f21);				

	angle_old_new = f1 - f21;
	float value_f21 = abs(value_a * cos(angle_old_new));
	F21.set_value(value_f21);

	/////////////////////////////////////////////////////////////////////

		// ���������� ���� ������� �� ������
	float f2, f22, f12;

	f2 = b->get_angle_energy();
	f12 = Vojtekh::get_angle_2_points(ball_b, ball_a);

	// ���������� ������� ������� �������
	t1 = f12 + PI / 2;
	t2 = f12 - PI / 2;

	if (abs(f2 - t1) <= (PI / 2))
		t = t1;
	else
		t = t2;

	// ���������� ������ ������� ���� 2
	f22 = f2 - (f2 - t) * 2;
	//b->set_angle_energy(f22);

	// ���������� ���� ���� ���� 2
	float value_b = b->get_energy();

	angle_old_new = f2 - f22;
	float value_f22 = abs(value_b * sin(angle_old_new));
	//b->set_energy(value_new_b);

	// ��������� ������� 2 ���� �� ����� �����
	Vojtekh::Vector_rad F22;
	F22.set_angle(f22);
	F22.set_value(value_f22);

	// ��������� ������� 1 ���� �� ����� 2 ����
	Vojtekh::Vector_rad F12;
	F12.set_angle(f12);

	angle_old_new = f2 - f12;
	float value_f12 = abs(value_b * cos(angle_old_new));
	F12.set_value(value_f12);

	// ��������� ���� ����� ������� �� ����� ����� �� �� ����� ������ �'���
	Vojtekh::Vector_rad F1, F2;
	F1 = F11 + F12;
	F2 = F22 + F21;

	// ����� ����� �������
	a->set_angle_energy(F1.get_angle());
	a->set_energy(F1.get_value());

	b->set_angle_energy(F2.get_angle());
	b->set_energy(F2.get_value());

	return true;
}

// ����������� ������� �����
void Game::init_borders()
{
	point2D start_pos = board->get_position_draw_rect();

	// ˳�� ����� ������������� �������
	Vojtekh::Line* elem = new Vojtekh::Line;
	elem->start = start_pos;
	elem->start.x += 66;
	elem->start.y += 94;
	
	elem->end = start_pos;
	elem->end.x += 66;
	elem->end.y += 576;

	elem->dir = Vojtekh::Line::direct::RIGHT;
	borders.push_back(elem);

	// ˳�� ������� ������������� �������
	elem = new Vojtekh::Line;
	elem->start = start_pos;
	elem->start.x += 1126;
	elem->start.y += 95;

	elem->end = start_pos;
	elem->end.x += 1126;
	elem->end.y += 575;

	elem->dir = Vojtekh::Line::direct::LEFT;
	borders.push_back(elem);

	// ˳� ������ �������������� �������
	elem = new Vojtekh::Line;
	elem->start = start_pos;
	elem->start.x += 94;
	elem->start.y += 66;

	elem->end = start_pos;
	elem->end.x += 565;
	elem->end.y += 66;

	elem->dir = Vojtekh::Line::direct::DOWN;
	borders.push_back(elem);

	elem = new Vojtekh::Line;
	elem->start = start_pos;
	elem->start.x += 628;
	elem->start.y += 66;

	elem->end = start_pos;
	elem->end.x += 1099;
	elem->end.y += 66;

	elem->dir = Vojtekh::Line::direct::DOWN;
	borders.push_back(elem);

	// ˳� ����� �������������� �������
	elem = new Vojtekh::Line;
	elem->start = start_pos;
	elem->start.x += 94;
	elem->start.y += 603;

	elem->end = start_pos;
	elem->end.x += 564;
	elem->end.y += 603;

	elem->dir = Vojtekh::Line::direct::UP;
	borders.push_back(elem);

	elem = new Vojtekh::Line;
	elem->start = start_pos;
	elem->start.x += 628;
	elem->start.y += 603;

	elem->end = start_pos;
	elem->end.x += 1099;
	elem->end.y += 603;

	elem->dir = Vojtekh::Line::direct::UP;
	borders.push_back(elem);

////////////////////////////////////////////////////////////////////////
	// ˳�� ����� ������� ��� ������� �����
	elem = new Vojtekh::Line;
	elem->start = start_pos;
	elem->start.x += 46;
	elem->start.y += 74;

	elem->end = start_pos;
	elem->end.x += 66;
	elem->end.y += 94;

	elem->dir = Vojtekh::Line::direct::RIGHT_UP;
	borders.push_back(elem);
	// ˳�� ����� ������� ��� ������ �����
	elem = new Vojtekh::Line;
	elem->start = start_pos;
	elem->start.x += 46;
	elem->start.y += 596;

	elem->end = start_pos;
	elem->end.x += 66;
	elem->end.y += 576;

	elem->dir = Vojtekh::Line::direct::RIGHT_DOWN;
	borders.push_back(elem);

	// ˳�� ������� ������� ��� ������� �����
	elem = new Vojtekh::Line;
	elem->start = start_pos;
	elem->start.x += 1146;
	elem->start.y += 75;

	elem->end = start_pos;
	elem->end.x += 1126;
	elem->end.y += 95;

	elem->dir = Vojtekh::Line::direct::LEFT_UP;
	borders.push_back(elem);
	// ˳�� ������� ������� ��� ������ �����
	elem = new Vojtekh::Line;
	elem->start = start_pos;
	elem->start.x += 1146;
	elem->start.y += 595;

	elem->end = start_pos;
	elem->end.x += 1126;
	elem->end.y += 575;

	elem->dir = Vojtekh::Line::direct::LEFT_DOWN;
	borders.push_back(elem);
///////////////////////////////////////////////
	// ˳�� ��������� ������� ��� ��� �����
	elem = new Vojtekh::Line;
	elem->start = start_pos;
	elem->start.x += 74;
	elem->start.y += 46;

	elem->end = start_pos;
	elem->end.x += 94;
	elem->end.y += 66;

	elem->dir = Vojtekh::Line::direct::LEFT_DOWN;
	borders.push_back(elem);
	
	// ˳�� ��������� ������� ��� �������� �����
	elem = new Vojtekh::Line;
	elem->start = start_pos;
	elem->start.x += 577;
	elem->start.y += 46;

	elem->end = start_pos;
	elem->end.x += 565;
	elem->end.y += 66;

	elem->dir = Vojtekh::Line::direct::RIGHT_DOWN;
	borders.push_back(elem);

	// ˳�� ��������� ������� ��� �������� �����
	elem = new Vojtekh::Line;
	elem->start = start_pos;
	elem->start.x += 616;
	elem->start.y += 46;

	elem->end = start_pos;
	elem->end.x += 628;
	elem->end.y += 66;

	elem->dir = Vojtekh::Line::direct::LEFT_DOWN;
	borders.push_back(elem);

	// ˳�� ��������� ������� ��� ����� �����
	elem = new Vojtekh::Line;
	elem->start = start_pos;
	elem->start.x += 1119;
	elem->start.y += 46;

	elem->end = start_pos;
	elem->end.x += 1099;
	elem->end.y += 66;

	elem->dir = Vojtekh::Line::direct::RIGHT_DOWN;
	borders.push_back(elem);

///////////////////////////////////////////////
	// ˳�� �������� ������� ��� ��� �����
	elem = new Vojtekh::Line;
	elem->start = start_pos;
	elem->start.x += 94;
	elem->start.y += 603;

	elem->end = start_pos;
	elem->end.x += 74;
	elem->end.y += 623;

	elem->dir = Vojtekh::Line::direct::RIGHT_UP;
	borders.push_back(elem);

	// ˳�� �������� ������� ��� �������� �����
	elem = new Vojtekh::Line;
	elem->start = start_pos;
	elem->start.x += 564;
	elem->start.y += 603;

	elem->end = start_pos;
	elem->end.x += 576;
	elem->end.y += 623;

	elem->dir = Vojtekh::Line::direct::RIGHT_UP;
	borders.push_back(elem);

	// ˳�� �������� ������� ��� �������� �����
	elem = new Vojtekh::Line;
	elem->start = start_pos;
	elem->start.x += 616;
	elem->start.y += 623;

	elem->end = start_pos;
	elem->end.x += 628;
	elem->end.y += 603;

	elem->dir = Vojtekh::Line::direct::LEFT_UP;
	borders.push_back(elem);

	// ˳�� �������� ������� ��� ����� �����
	elem = new Vojtekh::Line;
	elem->start = start_pos;
	elem->start.x += 1099;
	elem->start.y += 603;

	elem->end = start_pos;
	elem->end.x += 1119;
	elem->end.y += 623;

	elem->dir = Vojtekh::Line::direct::RIGHT_DOWN;
	borders.push_back(elem);
}

// ����������� ����� �����
void Game::init_holes()
{
	point2D start_pos = board->get_position_draw_rect();

	Ball_collision* hole = new Ball_collision;
	hole->set_size(SIZE_OF_BALL * 1.25);

	hole->set_pos(start_pos.x + 58, start_pos.y + 58 - 40);
	holes.push_back(hole);

	hole = new Ball_collision;
	hole->set_size(SIZE_OF_BALL * 1.25);
	hole->set_pos(start_pos.x + 1136, start_pos.y + 58 - 40);
	holes.push_back(hole);

	hole = new Ball_collision;
	hole->set_size(SIZE_OF_BALL * 1.25);
	hole->set_pos(start_pos.x + 58, start_pos.y + 613 - 40);
	holes.push_back(hole);

	hole = new Ball_collision;
	hole->set_size(SIZE_OF_BALL * 1.25);
	hole->set_pos(start_pos.x + 1136, start_pos.y + 613 - 40);
	holes.push_back(hole);

	hole = new Ball_collision;
	hole->set_size(SIZE_OF_BALL * 1.25);
	hole->set_pos(start_pos.x + 597, start_pos.y + 46 - 40);
	holes.push_back(hole);

	hole = new Ball_collision;
	hole->set_size(SIZE_OF_BALL * 1.25);
	hole->set_pos(start_pos.x + 597, start_pos.y + 625 - 40);
	holes.push_back(hole);
}

// ������� ��� ������� �� ������� �� ���������
bool Game::end_game() const { return is_end_game; }
int Game::get_moves() const { return move_number; }
int Game::get_fines() const { return fines_number; }
int Game::get_failures() const { return failures_number; }
