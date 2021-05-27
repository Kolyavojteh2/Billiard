#include "Ball_shape.h"
#include <iostream>

const float PI = 3.14159265;

Ball_shape::Ball_shape() : Draw_rect(), Ball_collision(), energy() 
{
	set_size(16);
	is_set_direct_ = false;
	is_select = false;
	set_position_shape(0, 0);
}
Ball_shape::Ball_shape(RenderWindow* w) : Draw_rect(w), Ball_collision(), energy() 
{
	set_size(16);
	is_set_direct_ = false;
	is_select = false;
	set_position_shape(0, 0);
}

void Ball_shape::set_energy(float n) { energy.set_value(abs(n)); }
void Ball_shape::set_angle_energy(float n) { energy.set_angle(n); }
void Ball_shape::set_texture(Texture* t) { Draw_rect::set_texture(t); }

void Ball_shape::set_position_shape(float x, float y)
{
	set_pos(x, y);

	float h, w;
	h = get_height();
	w = get_width();
	// Зміщуємо спрайт м'яча для відповідності дійсності відображення та колізії
	Draw_rect::set_position_draw_rect(x - (w / 2), y - (h / 2));
}

point2D Ball_shape::get_position_shape() const { return get_pos(); }

// Функція для обролення подій клавіатури та миші для м'яча
void Ball_shape::events_obj()
{
	// Затримка між подіями
	if (delay < 200)
	{
		int t;
		t = timer.getElapsedTime().asMilliseconds();
		timer.restart();
		delay += t;
		return;
	}

	// Визначення напрямку удара києм по шару
	if (Mouse::isButtonPressed(Mouse::Button::Left) && is_select && !is_set_direct_)
	{
		// Якщо натиснули лівою кнопкою миші, то вираховуємо кут руху шара
		// Визначаємо, зміщені до початку координат, координати миші у вікні
		Vector2i pos_mouse = Mouse::getPosition();
		Vector2i pos_wind = get_RenderWindow()->getPosition();
		pos_mouse.x -= pos_wind.x;
		pos_mouse.y -= pos_wind.y;

		// Визначаємо зміщені до початку координат, координати м'яча
		point2D pos_ball;
		pos_ball.x = get_pos_x() - pos_mouse.x;
		pos_ball.y = get_pos_y() + 40 - pos_mouse.y;

		// Визначення довжини від миші до м'яча
		float size_vec = sqrt(pos_ball.x*pos_ball.x + pos_ball.y*pos_ball.y);

		// Визначення косинусів та синусів до шара
		float cos = pos_ball.x / size_vec;
		float sin = pos_ball.y / size_vec;

		// Визначення кута і задання його як кут удару
		float a = Vojtekh::get_angle(sin, cos);
		energy.set_angle(a);

		// Ставимо прапорець що обрано кут удару
		is_set_direct_ = true;

		// Запускаємо таймер затримки
		timer.restart();
		delay = 0;
	}

	// Зняття вибору м'яча
	if (Mouse::isButtonPressed(Mouse::Button::Right) && is_select && is_set_direct_)
	{
		is_set_direct_ = false;

		timer.restart();
		delay = 0;
	}
}

void Ball_shape::draw() { Draw_rect::draw(); }

// Оновлення координат м'яча та його внутрішньої енергії
void Ball_shape::update(float time)
{
	if (!(int(energy.get_value() * 10000)))
		return;
	dx = energy.get_speed()*cos(energy.get_angle()) * time;
	dy = energy.get_speed()*sin(energy.get_angle()) * time;

	point2D pos_shape = get_position_shape();
	pos_shape.x += dx;
	pos_shape.y += dy;
	set_position_shape(pos_shape.x, pos_shape.y);

	energy.update(time);
}

Ball_shape::~Ball_shape() {}

float Ball_shape::get_energy() const { return energy.get_value(); }
float Ball_shape::get_angle_energy() const { return energy.get_angle(); }

// Вибір м'яча та перезапуск таймера затримки
void Ball_shape::select_ball(bool b) 
{
	if(this)
		is_select = b; 

	delay = 0;
	timer.restart();
}

// Зняття вибору м'яча та перезапуск таймера затримки
void Ball_shape::unselect_ball() 
{ 
	if (this->get_size() == 16)
	{
		is_select = false;
		is_set_direct_ = false;
	}


	delay = 0;
	timer.restart();
}

// Доступ до прапорців ззовні
bool Ball_shape::is_set_direct() const { return is_set_direct_; }
bool Ball_shape::is_selected() const { return is_select; }