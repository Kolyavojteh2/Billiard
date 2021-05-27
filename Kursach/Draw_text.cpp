#include "Draw_text.h"

// ���� �� ������
extern const char* dir_font;

// ��� �������� ����� ��� ������ ����������� �������������� �� �imes new roman
Draw_text::Draw_text(RenderWindow* w) : window(w)
{
	font.loadFromFile(dir_font);
	text.setFont(font);
	text.setFillColor(sf::Color::Black);
	text.setCharacterSize(20);
}

void Draw_text::set_text(string ss) { text.setString(ss); }
string Draw_text::get_text() const { return text.getString(); }
int Draw_text::get_size() const { return text.getCharacterSize(); }

void Draw_text::set_color(sf::Color c) { text.setFillColor(c); }
void Draw_text::set_size(int n) { text.setCharacterSize(n); }

Draw_text::~Draw_text() {}

void Draw_text::draw() { window->draw(text); }

void Draw_text::set_position(int x, int y) { text.setPosition(x, y); }