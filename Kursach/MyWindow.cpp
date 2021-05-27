#include "MyWindow.h"
#include "Menu.h"
#include <SFML/Graphics.hpp>

#include "Draw_text.h"
#include "Button.h"

#include "convert_string.h"

using namespace sf;
using namespace std;

// ��������� ������ ���� � ����� main.cpp
extern const int window_width;
extern const int window_height;

// ����� �� ������� ������, ���� �� ���������� ����
extern const char* dir_button;
extern const char* dir_background_additional_window;
extern const char* dir_background_intro;
extern const char* dir_intro_s;
extern const char* dir_fone_s;

MyWindow::MyWindow(VideoMode mode, string name) : state(0)
{
	// ��������� ���� �� ������� ���� ������� ��������� ������
	window = new RenderWindow(mode, name);
	window->setFramerateLimit(144);
	icon = new Image;
	icon->loadFromFile("res/icon.png");
	window->setIcon(icon->getSize().x, icon->getSize().y, icon->getPixelsPtr());

	// ����������� �����
	click_buf.loadFromFile(dir_ball_s);
	s_click.setBuffer(click_buf);
}

// ������ �� ��'���� ����
RenderWindow* MyWindow::get_window() const { return window; }

MyWindow::~MyWindow()
{
	if (window)
		delete window;

	if (icon)
		delete icon;
}

void MyWindow::event_handling()
{
// ������ �������
	Clock timer;
	float time = 0;

// ���������� ���� SFML
	sf::Event event;
	while (window->isOpen())
	{
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window->close();
		}

		// ������� ����� ��������
		switch (state)
		{
		// ���������� ����(�����)
		case 0:
		{
			create_intro();
			
			// ������� �� ����� 1 (������� ��������)
			state = 1;
			break;
		}

		// ������� ��������
		case 1:
		{
			// �������� ���� ������� 5 ������, ���� ���� ���� �������� �������� �� ����� ��������� ����(2)
			if (time > 5000)
				state = 2;

			time += timer.getElapsedTime().asMilliseconds();
			timer.restart();
			
			// ��������� ����
			menu->draw();

			// ���� ���� ������� �� ���� ��������� ����(2)
			if (state == 2)
			{
				// ��������� ������� �������� �� ��������� ������� ����
				delete_intro();
				create_main_menu();

				// ���������� ������ ������
				fone.openFromFile(dir_fone_s);
				fone.setLoop(true);
				fone.setVolume(10);
				fone.play();
			}

			break;
		}

		// ������� ����
		case 2:
		{
			// ������� ���� �� ��������� ������� ���� ��������� ����
			window->clear();
			state = menu->events_obj();
			menu->draw();

			// ���� ���� ������� ������� ���� �� ���(3)
			if (state == 3)
			{
				// ��������� ���� �� ��������� ���
				delete_main_menu();
				create_game();
			}

			// ���� ���� ������� �� ���������� �� ���(4)
			if (state == 4)
			{
				// ��������� ������ �������, ��� ��������� ��������� ����
				create_manual();
			}

			// ���� ���� ������� �� about(5)
			if (state == 5)
			{
				// ��������� ������ about, ��� ��������� ��������� ����
				create_about();
			}

			break;
		}

		// ���� ���
		case 3:			
		{
			// ��������� �������� � ������� �� �������� ���� ��������
			time = timer.getElapsedTime().asMicroseconds();
			timer.restart();
			time /= 800;

			// ���������� ���� ���
			state = game->event_obj();

			// ��������� ����� ���
			game->update(time);

			// ���������
			game->draw();

			// ���� ���� �������� ������� �� 2(������� ����)
			if (state == 2)
			{
				// ��������� ��� �� ��������� ����
				create_main_menu();
				delete_game();
			}
			
			// ���� ���� ������� �� ����� ���(7)
			if (state == 7)
			{
				// ��������� ������ ��� ��������� ���
				create_end_game();
			}
			break;
		}

		case 4:
		{
			// ������� ���� �� ��������� ��������� ����
			window->clear();
			menu->draw();

			// ������ ������� ���� ������ ������� �� ���� ���������
			state = additional_window->events_obj();
			additional_window->draw();

			// ���� ���� ��������� �� 2(������� ����) ��������� ���� �������
			if (state == 2)
			{
				delete_manual();
			}
			break;
		}


		case 5:
		{
			// ������� ���� �� ��������� ��������� ����
			window->clear();
			menu->draw();

			// ������ ������� ���� ������ about �� ���� ���������
			state = additional_window->events_obj();
			additional_window->draw();
			
			// ���� ���� ��������� �� 2(������� ����) ��������� ���� �������
			if (state == 2)
			{
				delete_about();
			}
			break;
		}

		// ����� � ��� ����� ������
		case 6:
		{
			// ��������� ��� �� ��������� ���� ���� ���� �
			delete_main_menu();
			delete_game();
			
			// �������� ���� 
			window->close();
			break;
		}

		case 7:
		{
			// ��������� �������� � ������� �� �������� ���� ��������
			time = timer.getElapsedTime().asMicroseconds();
			timer.restart();
			time /= 800;

			// ���������� ���� ���
			game->update(time);

			// ���������
			game->draw();

			// ��������� �� ������� ���� ������ ����������
			state = additional_window->events_obj();
			additional_window->draw();

			// ������� �� ��������� ����
			if (state == 2)
			{
				delete_end_game();
				delete_game();

				create_main_menu();
			}

			break;
		}

		}

		// ³���������� ����
		window->display();
	}
}

// ����������� ����� �� ��������� ������� ��������
void MyWindow::create_intro()
{
	// ��������� ������� �� ������� ����������� �����
	menu = new Menu(window, &s_click);
	menu->set_jump_state(1);

	// ������� �������� �������
	Texture* t = new Texture;
	t->loadFromFile(dir_background_intro);
	menu->set_background(t);

	// ����������� ������
	Draw_text* t1 = new Draw_text(window);
	t1->set_color(sf::Color::White);
	t1->set_size(72);
	t1->set_text("Billiard");
	t1->set_position(540, 140);
	menu->add_Draw_text(t1);

	t1 = new Draw_text(window);
	t1->set_color(sf::Color::White);
	t1->set_size(40);
	t1->set_text("Develouper: Vojtekh Mykola SP-225");
	t1->set_position(330, 220);
	menu->add_Draw_text(t1);

	t1 = new Draw_text(window);
	t1->set_color(sf::Color::White);
	t1->set_size(40);
	t1->set_text("2020");
	t1->set_position(600, 660);
	menu->add_Draw_text(t1);

	// ����������� �� ������ ������ ������
	fone.openFromFile(dir_intro_s);
	fone.setVolume(70);
	fone.play();
}

// ��������� ������� ��������
void MyWindow::delete_intro()
{
	if (menu)
		delete menu;
	menu = nullptr;
}

// ����������� ����� �� ��������� ��������� ����
void MyWindow::create_main_menu()
{
	// ��������� ��������� ����
	menu = new Menu(window, &s_click);
	menu->set_jump_state(2);

	// ������� �������� �������
	Texture* t = new Texture;
	t->loadFromFile(dir_background_intro);
	menu->set_background(t);

	// ����������� ������
	Draw_text* t1 = new Draw_text(window);
	t1->set_color(sf::Color::White);
	t1->set_size(40);
	t1->set_text("V1.0");
	t1->set_position(1200, 750);
	menu->add_Draw_text(t1);

	// ����������� ������
	Texture* t_button = new Texture;
	t_button->loadFromFile(dir_button);
	Button* b = new Button(window);
	b->set_texture(t_button);
	b->set_size(72);
	b->set_color(Color::White);
	b->set_text("New Game");
	b->set_position(650, 150);
	b->set_jump(3);
	menu->add_Button(b);

	t_button = new Texture;
	t_button->loadFromFile(dir_button);
	b = new Button(window);
	b->set_texture(t_button);
	b->set_size(72);
	b->set_color(Color::White);
	b->set_text("How to play");
	b->set_position(650, 270);
	b->set_jump(4);
	menu->add_Button(b);

	t_button = new Texture;
	t_button->loadFromFile(dir_button);
	b = new Button(window);
	b->set_texture(t_button);
	b->set_size(72);
	b->set_color(Color::White);
	b->set_text("About");
	b->set_position(650, 390);
	b->set_jump(5);
	menu->add_Button(b);

	t_button = new Texture;
	t_button->loadFromFile(dir_button);
	b = new Button(window);
	b->set_texture(t_button);
	b->set_size(72);
	b->set_color(Color::White);
	b->set_text("Quit");
	b->set_position(650, 510);
	b->set_jump(6);
	menu->add_Button(b);

	// ������ ������ ������
	if (fone.getStatus() != sf::SoundSource::Status::Playing)
		fone.play();
}

// ��������� ��������� ����
void MyWindow::delete_main_menu()
{
	if (menu)
		delete menu;
	menu = nullptr;
}

// ��������� �� ����������� ���
void MyWindow::create_game()
{
	game = new Game(window);
	game->start();
}

// ��������� ���
void MyWindow::delete_game()
{
	if (game)
		delete game;
	game = nullptr;

}

// ��������� ������ ���������� �� ���
void MyWindow::create_manual()
{
	// ��������� ������
	additional_window = new Menu(window, &s_click);
	additional_window->set_jump_state(4);

	// ������� �������� �������
	Texture* texture = new Texture;
	texture->loadFromFile(dir_background_additional_window);
	additional_window->set_background(texture);

	// ����������� ������
	Draw_text* t = new Draw_text(window);
	t->set_color(sf::Color::White);
	t->set_size(72);
	t->set_text("How to play");
	t->set_position(500, 60);
	additional_window->add_Draw_text(t);

	t = new Draw_text(window);
	t->set_color(sf::Color::White);
	t->set_size(20);
	t->set_text("The essence of the game is to score all the balls in the hole with the right punches. The game consists of moves.");
	t->set_position(150, 150);
	additional_window->add_Draw_text(t);

	t = new Draw_text(window);
	t->set_color(sf::Color::White);
	t->set_size(20);
	t->set_text("During each turn you have to hit a stick with a ball, and for this you need to: choose the ball, choose the angle of impact,");
	t->set_position(150, 172);
	additional_window->add_Draw_text(t);

	t = new Draw_text(window);
	t->set_color(sf::Color::White);
	t->set_size(20);
	t->set_text("choose the force of the impact and strike the ball.");
	t->set_position(150, 194);
	additional_window->add_Draw_text(t);

	t = new Draw_text(window);
	t->set_color(sf::Color::White);
	t->set_size(20);
	t->set_text("1. Choosing the ball.");
	t->set_position(150, 230);
	additional_window->add_Draw_text(t);

	t = new Draw_text(window);
	t->set_color(sf::Color::White);
	t->set_size(20);
	t->set_text("First you need to choose the ball that you will beat, for this simply click on it LMB. To deselect the ball, press the RMB.");
	t->set_position(150, 252);
	additional_window->add_Draw_text(t);

	t = new Draw_text(window);
	t->set_color(sf::Color::White);
	t->set_size(20);
	t->set_text("2. Choosing the angle of impact.");
	t->set_position(150, 290);
	additional_window->add_Draw_text(t);

	t = new Draw_text(window);
	t->set_color(sf::Color::White);
	t->set_size(20);
	t->set_text("After selecting a layer, select an angle of rotation by rotating the mouse around the layer and clicking LMB.");
	t->set_position(150, 312);
	additional_window->add_Draw_text(t);

	t = new Draw_text(window);
	t->set_color(sf::Color::White);
	t->set_size(20);
	t->set_text("Next, select the impact force to do this, click on the force scale to the left of the desired value.");
	t->set_position(150, 334);
	additional_window->add_Draw_text(t);

	t = new Draw_text(window);
	t->set_color(sf::Color::White);
	t->set_size(20);
	t->set_text("3. Strike with a cane.");
	t->set_position(150, 370);
	additional_window->add_Draw_text(t);

	t = new Draw_text(window);
	t->set_color(sf::Color::White);
	t->set_size(20);
	t->set_text("After selecting the force, click SPACE to hit.");
	t->set_position(150, 392);
	additional_window->add_Draw_text(t);

	t = new Draw_text(window);
	t->set_color(sf::Color::White);
	t->set_size(20);
	t->set_text("After the impact, you have to wait for all the balls to stop, and only then will the course be completed.");
	t->set_position(150, 430);
	additional_window->add_Draw_text(t);

	t = new Draw_text(window);
	t->set_color(sf::Color::White);
	t->set_size(20);
	t->set_text("The game will continue until there are 2 or more balls on the table.");
	t->set_position(150, 452);
	additional_window->add_Draw_text(t);

	t = new Draw_text(window);
	t->set_color(sf::Color::White);
	t->set_size(20);
	t->set_text("Exit to main menu.");
	t->set_position(150, 490);
	additional_window->add_Draw_text(t);

	t = new Draw_text(window);
	t->set_color(sf::Color::White);
	t->set_size(20);
	t->set_text("Click ESCAPE to access the main menu.");
	t->set_position(150, 512);
	additional_window->add_Draw_text(t);

	// ����������� ������
	Texture* t_button = new Texture;
	t_button->loadFromFile(dir_button);
	Button* b = new Button(window);
	b->set_texture(t_button);
	b->set_size(72);
	b->set_color(Color::White);
	b->set_text("OK");
	b->set_position(650, 625);
	b->set_jump(2);
	additional_window->add_Button(b);
}

// ��������� ���������� �� ���
void MyWindow::delete_manual()
{
	if (additional_window)
		delete additional_window;
	additional_window = nullptr;
}

// ��������� ������ about 
void MyWindow::create_about()
{
	// ��������� ������
	additional_window = new Menu(window, &s_click);
	additional_window->set_jump_state(5);

	// ������� �������� �������
	Texture* texture = new Texture;
	texture->loadFromFile(dir_background_additional_window);
	additional_window->set_background(texture);

	// ����������� ������
	Draw_text* t = new Draw_text(window);
	t->set_color(sf::Color::White);
	t->set_size(72);
	t->set_text("About");
	t->set_position(570, 60);
	additional_window->add_Draw_text(t);

	t = new Draw_text(window);
	t->set_color(sf::Color::White);
	t->set_size(40);
	t->set_text("Term paper");
	t->set_position(570, 170);
	additional_window->add_Draw_text(t);

	t = new Draw_text(window);
	t->set_color(sf::Color::White);
	t->set_size(40);
	t->set_text("sbj : object oriented programming");
	t->set_position(390, 220);
	additional_window->add_Draw_text(t);

	t = new Draw_text(window);
	t->set_color(sf::Color::White);
	t->set_size(40);
	t->set_text("by student Vojtekh Mykola");
	t->set_position(450, 270);
	additional_window->add_Draw_text(t);

	t = new Draw_text(window);
	t->set_color(sf::Color::White);
	t->set_size(40);
	t->set_text("SP-225 form");
	t->set_position(560, 320);
	additional_window->add_Draw_text(t);

	// ����������� ������
	Texture* t_button = new Texture;
	t_button->loadFromFile(dir_button);
	Button* b = new Button(window);
	b->set_texture(t_button);
	b->set_size(72);
	b->set_color(Color::White);
	b->set_text("OK");
	b->set_position(650, 625);
	b->set_jump(2);
	additional_window->add_Button(b);
}

// ��������� ������ about 
void MyWindow::delete_about()
{
	if (additional_window)
		delete additional_window;
	additional_window = nullptr;
}

// ��������� ������ ��� ����� ���
void MyWindow::create_end_game()
{
	// ��������� ������
	additional_window = new Menu(window, &s_click);
	additional_window->set_jump_state(7);

	// ������� �������� �������
	Texture* texture = new Texture;
	texture->loadFromFile(dir_background_additional_window);
	additional_window->set_background(texture);

	// ��������� ������� ��� ���������� ���
	int moves = game->get_moves();
	int failures = game->get_failures();
	int fines = game->get_fines();

	// ����������� ������
	Draw_text* t = new Draw_text(window);
	t->set_color(sf::Color::White);
	t->set_size(72);
	t->set_text("Game over");
	t->set_position(530, 60);
	additional_window->add_Draw_text(t);

	t = new Draw_text(window);
	t->set_color(sf::Color::White);
	t->set_size(56);
	t->set_text("Number of moves:");
	t->set_position(150, 180);
	additional_window->add_Draw_text(t);

	t = new Draw_text(window);
	t->set_color(sf::Color::White);
	t->set_size(56);
	t->set_text("Number of failures:");
	t->set_position(150, 250);
	additional_window->add_Draw_text(t);

	t = new Draw_text(window);
	t->set_color(sf::Color::White);
	t->set_size(56);
	t->set_text("Number of fines:");
	t->set_position(150, 320);
	additional_window->add_Draw_text(t);

	t = new Draw_text(window);
	t->set_color(sf::Color::White);
	t->set_size(56);
	string ss;
	ss = Vojtekh::convert_string::to_string<int>(moves);
	t->set_text(ss);
	t->set_position(850, 180);
	additional_window->add_Draw_text(t);

	t = new Draw_text(window);
	t->set_color(sf::Color::White);
	t->set_size(56);
	ss = Vojtekh::convert_string::to_string<int>(failures);
	t->set_text(ss);
	t->set_position(850, 250);
	additional_window->add_Draw_text(t);

	t = new Draw_text(window);
	t->set_color(sf::Color::White);
	t->set_size(56);
	ss = Vojtekh::convert_string::to_string<int>(fines);
	t->set_text(ss);
	t->set_position(850, 320);
	additional_window->add_Draw_text(t);

	// ����������� ������
	Texture* t_button = new Texture;
	t_button->loadFromFile(dir_button);
	Button* b = new Button(window);
	b->set_texture(t_button);
	b->set_size(72);
	b->set_color(Color::White);
	b->set_text("OK");
	b->set_position(650, 625);
	b->set_jump(2);
	additional_window->add_Button(b);
}

// ��������� ������ ���������� ���
void MyWindow::delete_end_game()
{
	if (additional_window)
		delete additional_window;
	additional_window = nullptr;
}