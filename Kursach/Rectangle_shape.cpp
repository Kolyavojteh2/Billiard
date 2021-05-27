#include "Rectangle_shape.h"

Rectangle_shape::Rectangle_shape() : Draw_rect(), Rectangle_collision(){}
Rectangle_shape::Rectangle_shape(RenderWindow* w) : Draw_rect(w), Rectangle_collision(){}

void Rectangle_shape::set_texture(Texture*t) 
{ 
	Draw_rect::set_texture(t);
	Rectangle_collision::set_width(t->getSize().x);
	Rectangle_collision::set_height(t->getSize().y);
}
void Rectangle_shape::set_position_shape(float x, float y)
{
	set_start_point(x, y);
	set_position_draw_rect(x, y);
}
point2D Rectangle_shape::get_position_shape() const { return get_position_draw_rect(); }

// Функція оброблення подій пуста так як немає типового призначення для прямокутника
void Rectangle_shape::events_obj() {}
void Rectangle_shape::draw() { Draw_rect::draw(); }
void Rectangle_shape::update(float time) {}

Rectangle_shape::~Rectangle_shape() {}