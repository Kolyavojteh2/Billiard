#include "Button.h"

Button::Button(RenderWindow* w) : Rectangle_shape(w), Draw_text(w), jump_to_state(0) {}

// Задання позиції виконується точкою центра кнопки
void Button::set_position(int x, int y)
{
	float height = Draw_rect::get_height();
	float width = Draw_rect::get_width();

	float t_h = Draw_text::get_size();
	float t_w = t_h / 1.2;

	string ss = Draw_text::get_text();

	// Зміщення колізії, тексту та спрайта кнопки
	Draw_rect::set_position_draw_rect(x - width / 2, y - height / 2);
	Rectangle_shape::set_start_point(x - width / 2, y - height*0.1);
	Rectangle_shape::set_height(height);
	Rectangle_shape::set_width(width);
	Draw_text::set_position(x - t_w / 2 * (ss.size() + 1) / 2, y - t_h / 4 * 3);
}

void Button::set_position(point2D p) { set_position(p.x, p.y); }

point2D Button::get_position() const 
{ 
	float height = Draw_rect::get_height();
	float width = Draw_rect::get_width();

	point2D r;
	r.x = Draw_rect::get_position_draw_rect().x + width / 2;
	r.y = Draw_rect::get_position_draw_rect().y + height / 2;

	return r;
}

void Button::set_texture(Texture* t) 
{ 
	Draw_rect::set_texture(t); 
	texture = t; 
}

void Button::draw()
{
	Rectangle_shape::draw();
	Draw_text::draw();
}

int Button::get_jump() const { return jump_to_state; }

Button::~Button() 
{ 
	if (texture)
	{
		delete texture;
		texture = nullptr;
	}
}

int Button::get_height() const { return Draw_rect::get_height(); }
int Button::get_width() const {	return Draw_rect::get_width(); }

bool Button::is_collision(float x, float y) { return Rectangle_shape::is_collision(x, y); }

void Button::set_jump(int n) { jump_to_state = n; }