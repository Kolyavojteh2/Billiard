#pragma once

#include <SFML/Graphics.hpp>
#include <string>

using namespace std;
using namespace sf;

// ���� ��� ��������� ������ � ���
class Draw_text
{
	Font font;		// ����� ������
	Text text;		// �����

	RenderWindow* window;	// ���� ��� ���������

public:
	Draw_text(RenderWindow* w);
	
	// ������� ��������� ������
	void set_text(string ss);
	void set_color(sf::Color c);
	void set_size(int n);

	// ���������� ������
	string get_text() const;
	// ���������� ������� ���� � ����� (��������� ��� ����������� ������ � �������)
	int get_size() const;

	void draw();

	~Draw_text();

	// ������� �������
	void set_position(int x, int y);
};