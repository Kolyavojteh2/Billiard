#include "Select_Ball.h"
#include <vector>

// ����� �� �������
extern const char* dir_select_ball;

Select_ball::Select_ball(RenderWindow* w) : s(nullptr), window(w), block(false), delay(0)
{
	cirkle = new Draw_rect(w);

	texture_cirkle = new Texture;
	texture_cirkle->loadFromFile(dir_select_ball);
	cirkle->set_texture(texture_cirkle);
}

// ������� ������� ���� ������ ���������� �� ��������� ��� � ��
void Select_ball::event_obj(bool is_end_move) 
{
	// �������� �� ������
	if (delay < 500)
	{
		int t;
		t = timer.getElapsedTime().asMilliseconds();
		timer.restart();
		delay += t;
		return;
	}

	// ���� ��� �� ���������� �� �� ��������� �������
	if (!is_end_move)
		return;

	// �������� �� ���� �������� ���
	if (s && block)
	{
		// ������ ������
		if (s->get_size() != 16)
		{
			block = false;

			// ���������� �������
			timer.restart();
			delay = 0;
		}
	}
	// ���� ���� ��� �� ���� ������ ���� � ������������� ������
	else if (!s)
	{
		block = false;

		timer.restart();
		delay = 0;
	}

	// ���� ��������� ��� ����� ���� �� �'��� � ���� �� �����������
	if (Mouse::isButtonPressed(Mouse::Button::Left) && s && (block == false))
	{
		// �������� �'�� � ������� ����
		s->select_ball();
		block = true;

		// ������������� ������
		timer.restart();
		delay = 0;
	}
	
	// ���� ��������� ����� ������ ����, ��� ����� ����������� ���� �'��� �� �������� ����� �'��� �� ������� �� ������ ���� �'��� �� ������������ ����
	if (Mouse::isButtonPressed(Mouse::Button::Right) && block)
	{
		if (s)
			if (!s->is_set_direct())
			{
				s->unselect_ball();
				block = false;

				// ���������� �������
				timer.restart();
				delay = 0;
			}
	}

}

// ������� ������ ����� �'���� � �� ��������� ���� ����
void Select_ball::update(vector<Ball_shape*> arr, bool is_end_move)
{
	// ���� �������� �� �'�� ������ � �'�� �� �������� ������ ����
	if (s)
		if (!s->is_selected())
			block = false;

	// ���� ��� �� �� ���������� �������� ��������� �����
	if (!is_end_move)
		return;

	// ���� ���� �'��� ������������, �� �������� ����� �'��� �� ���� ��������� 
	if (block == false)
	{
		// ���������� ���� � ������ �'�� �� ���� �� ���������
		Ball_shape* selected = nullptr;
		for (auto it = arr.begin(); it != arr.end(); it++)
		{
			if (*it == nullptr)
				continue;

			auto mouse = Mouse::getPosition();
			auto pos_window = window->getPosition();

			mouse.x -= pos_window.x;
			mouse.y -= pos_window.y;

			// �������� �� ���������� ���������
			if ((*it)->get_size() != 16)
				continue;

			bool coll = (*it)->is_collision(mouse.x, mouse.y);
			if (coll)
				selected = (*it);
		}

		// ���� �� �� ������ �� �'�� �� ������ ������ ������ �'��� � �������� �������� �� ����
		if (selected == nullptr)
		{
			s = nullptr;
			remove_position();
			return;
		}

		// ���� �� ������ �� ��� �������� ���� �������� �� ������ ������� ������� ����
		s = selected;
		cirkle->set_position_draw_rect(s->get_pos().x - cirkle->get_width() / 2, s->get_pos().y - cirkle->get_height() / 2);
	}
}

void Select_ball::draw()
{
	if (cirkle)
		if(!((cirkle->get_position_draw_rect().x == -100) && (cirkle->get_position_draw_rect().y == -100)))
			cirkle->draw();
}

Select_ball::~Select_ball()
{
	if (cirkle)
		delete cirkle;
	if (texture_cirkle)
		delete texture_cirkle;
}

Ball_shape* Select_ball::get_Ball() const { return s; }

void Select_ball::remove_position() { cirkle->set_position_draw_rect(-100, -100); }