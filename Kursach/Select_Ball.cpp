#include "Select_Ball.h"
#include <vector>

// Ўл€хи до текстур
extern const char* dir_select_ball;

Select_ball::Select_ball(RenderWindow* w) : s(nullptr), window(w), block(false), delay(0)
{
	cirkle = new Draw_rect(w);

	texture_cirkle = new Texture;
	texture_cirkle->loadFromFile(dir_select_ball);
	cirkle->set_texture(texture_cirkle);
}

// ‘ункц≥€ обробки под≥й приймаЇ ≥нформац≥ю чи зак≥нчивс€ х≥д в гр≥
void Select_ball::event_obj(bool is_end_move) 
{
	// «атримка м≥ж под≥€ми
	if (delay < 500)
	{
		int t;
		t = timer.getElapsedTime().asMilliseconds();
		timer.restart();
		delay += t;
		return;
	}

	// якщо х≥д не завершивс€ ще то завершуЇмо обробку
	if (!is_end_move)
		return;

	// ѕерев≥рка чи ≥снуЇ вибраний шар
	if (s && block)
	{
		// «н€тт€ вибору
		if (s->get_size() != 16)
		{
			block = false;

			// ѕерезапуск таймеру
			timer.restart();
			delay = 0;
		}
	}
	// якщо м€ча вже не ≥снуЇ зн≥маЇмо виб≥р ≥ перезапускаЇмо таймер
	else if (!s)
	{
		block = false;

		timer.restart();
		delay = 0;
	}

	// якщо натиснули л≥ву кнопу миш≥ на м'€чу ≥ виб≥р не заблоковано
	if (Mouse::isButtonPressed(Mouse::Button::Left) && s && (block == false))
	{
		// ¬ибираЇмо м'€ч ≥ ставимо блок
		s->select_ball();
		block = true;

		// ѕерезапускаЇмо таймер
		timer.restart();
		delay = 0;
	}
	
	// якщо натиснули праву кнопку миш≥, при цьому заблоковано виб≥р м'€ча та напр€мок удару м'€ча не вибрано то зн≥маЇмо виб≥р м'€ча та розблоковуЇмо виб≥р
	if (Mouse::isButtonPressed(Mouse::Button::Right) && block)
	{
		if (s)
			if (!s->is_set_direct())
			{
				s->unselect_ball();
				block = false;

				// ѕерезапуск таймера
				timer.restart();
				delay = 0;
			}
	}

}

// ‘ункц≥€ приймаЇ масив м'€ч≥в ≥ та прапорець к≥нц€ ходу
void Select_ball::update(vector<Ball_shape*> arr, bool is_end_move)
{
	// якщо вказ≥вник на м'€ч д≥йсний ≥ м'€ч не вибраний зн≥маЇмо блок
	if (s)
		if (!s->is_selected())
			block = false;

	// якщо х≥д ще не завершивс€ зак≥нчуЇмо оновленн€ даних
	if (!is_end_move)
		return;

	// якщо виб≥р м'€ча розблоковано, то починаЇмо пошук м'€ча на €кий натиснули 
	if (block == false)
	{
		// ѕеребираЇмо цикл ≥ шукаЇмо м'€ч на €кий ми натиснули
		Ball_shape* selected = nullptr;
		for (auto it = arr.begin(); it != arr.end(); it++)
		{
			if (*it == nullptr)
				continue;

			auto mouse = Mouse::getPosition();
			auto pos_window = window->getPosition();

			mouse.x -= pos_window.x;
			mouse.y -= pos_window.y;

			// ѕерев≥рка на адекватний указатель
			if ((*it)->get_size() != 16)
				continue;

			bool coll = (*it)->is_collision(mouse.x, mouse.y);
			if (coll)
				selected = (*it);
		}

		// якщо ми не попали на м'€ч то ховаЇмо спрайт вибору м'€ча ≥ записуЇмо вказ≥вник на нуль
		if (selected == nullptr)
		{
			s = nullptr;
			remove_position();
			return;
		}

		// якщо ми попали на шар збер≥гаЇмо його вказ≥вник та задаЇмо позиц≥ю спрайта кола
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