#include "Draw_rect.h"

using namespace std;

Draw_rect::Draw_rect() : window(nullptr) {}
Draw_rect::Draw_rect(RenderWindow* w) : window(w) {}

RenderWindow* Draw_rect::get_RenderWindow() const { return window; }

point2D Draw_rect::get_position_draw_rect() const 
{ 
	Vector2f a = sprite.getPosition(); 
	point2D res;
	res.x = a.x;
	res.y = a.y;

	return res;
}

float Draw_rect::get_height() const
{
	IntRect a = sprite.getTextureRect();
	return a.height;
}
float Draw_rect::get_width() const
{
	IntRect a = sprite.getTextureRect();
	return a.width;
}

// Повернення значення повороту спрайта
float Draw_rect::get_rotate_angle() const { return sprite.getRotation(); }

void Draw_rect::set_window(RenderWindow* w) { window = w; }
void Draw_rect::set_position_draw_rect(int x, int y) { sprite.setPosition(x, y); }

// Задання значення повороту спрайта
void Draw_rect::set_rotate_angle(float angle) { sprite.rotate(angle); }
void Draw_rect::set_texture(Texture* t) { sprite.setTexture((*t)); }

void Draw_rect::move(float x, float y) { sprite.move(x, y); }
void Draw_rect::draw() { window->draw(sprite); }

Draw_rect::~Draw_rect() {}