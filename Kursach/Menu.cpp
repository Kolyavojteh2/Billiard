#include "Menu.h"

Menu::Menu(RenderWindow* w, Sound* s) : window(w), s_click(s) {	background = new Draw_rect(w); }

void Menu::add_Button(Button* b) { buttons.push_back(b); }
void Menu::add_Draw_text(Draw_text* t) { textes.push_back(t); }

// Îáğîáêà ïîä³é ìåíş
int Menu::events_obj()
{
	// ßêùî íàòèñíóëè ËÊÌ, òî øóêàºìî êíîïêó íà ÿêó íàòèñíóëè
	if (Mouse::isButtonPressed(Mouse::Button::Left))
	{
		for (auto it = buttons.begin(); it != buttons.end(); it++)
		{
			if ((*it)->is_collision(Mouse::getPosition().x - window->getPosition().x, Mouse::getPosition().y - window->getPosition().y))
			{
				// ßêùî çíàéøëè êíîïêó, òî ïîâåğòàºìî çíà÷åííÿ ñòàíó â ÿêèé íåîáõ³äíî ïåğåéòè ãğ³
				s_click->play();
				return (*it)->get_jump();
			}
		}
	}

	// ßêùî êíîïêè íå çíàéøëè çàëèøàºìîñÿ â öüîìó æ ìåíş
	return this_state;
}

void Menu::draw()
{
	background->draw();

	for (auto it = buttons.begin(); it != buttons.end(); it++)
	{
		if (*it)
			(*it)->draw();
	}

	for (auto it = textes.begin(); it != textes.end(); it++)
	{
		if (*it)
			(*it)->draw();
	}
}

Menu::~Menu()
{
	for (auto it = buttons.begin(); it != buttons.end(); it++)
	{
		if (*it)
		{
			delete (*it);
			(*it) = nullptr;
		}
	}

	for (auto it = textes.begin(); it != textes.end(); it++)
	{
		if (*it)
		{
			delete (*it);
			(*it) = nullptr;
		}
	}

	if (background)
	{
		delete background;
		background = nullptr;
	}

	if (t_background)
	{
		delete t_background;
		t_background = nullptr;
	}
}

void Menu::set_background(Texture* t)
{
	t_background = t;
	background->set_texture(t);
}

int Menu::get_jump_state() const { return this_state; }
void Menu::set_jump_state(int n) { this_state = n; }