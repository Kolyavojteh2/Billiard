#include "MyWindow.h"
#include "Menu.h"
#include <SFML/Graphics.hpp>

#include "Draw_text.h"
#include "Button.h"

#include "convert_string.h"

using namespace sf;
using namespace std;

// Константні розміри вікна з файлу main.cpp
extern const int window_width;
extern const int window_height;

// Шляхи до текстур кнопок, фону та додаткових вікон
extern const char* dir_button;
extern const char* dir_background_additional_window;
extern const char* dir_background_intro;
extern const char* dir_intro_s;
extern const char* dir_fone_s;

MyWindow::MyWindow(VideoMode mode, string name) : state(0)
{
	// Створення вікна та задання йому частоти оновлення екрану
	window = new RenderWindow(mode, name);
	window->setFramerateLimit(144);
	icon = new Image;
	icon->loadFromFile("res/icon.png");
	window->setIcon(icon->getSize().x, icon->getSize().y, icon->getPixelsPtr());

	// Ініціалізація звуку
	click_buf.loadFromFile(dir_ball_s);
	s_click.setBuffer(click_buf);
}

// Доступ до об'єкту вікна
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
// Запуск таймеру
	Clock timer;
	float time = 0;

// Оброблення подій SFML
	sf::Event event;
	while (window->isOpen())
	{
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window->close();
		}

		// Система станів програми
		switch (state)
		{
		// Початковий стан(старт)
		case 0:
		{
			create_intro();
			
			// Перехід до стану 1 (Вступна заставка)
			state = 1;
			break;
		}

		// Вступна заставка
		case 1:
		{
			// Заставка буде тривати 5 секунд, після чого стан програми зміниться на номер головного меню(2)
			if (time > 5000)
				state = 2;

			time += timer.getElapsedTime().asMilliseconds();
			timer.restart();
			
			// Малювання меню
			menu->draw();

			// Якщо стан змінився на стан головного меню(2)
			if (state == 2)
			{
				// Видаляємо вступну заставку та створюємо головне меню
				delete_intro();
				create_main_menu();

				// Ініціалізуємо фонову музику
				fone.openFromFile(dir_fone_s);
				fone.setLoop(true);
				fone.setVolume(10);
				fone.play();
			}

			break;
		}

		// Головне меню
		case 2:
		{
			// Очищуємо вікно та запускаємо обробку подій головного меню
			window->clear();
			state = menu->events_obj();
			menu->draw();

			// Якщо після обробки змінився стан на гру(3)
			if (state == 3)
			{
				// Видаляємо меню та створюємо гру
				delete_main_menu();
				create_game();
			}

			// Якщо стан змінився на інструкцію до гри(4)
			if (state == 4)
			{
				// Створюємо підвікно мануала, без видалення головного меню
				create_manual();
			}

			// Якщо стан змінився на about(5)
			if (state == 5)
			{
				// Створюємо підвікно about, без видалення головного меню
				create_about();
			}

			break;
		}

		// Стан гри
		case 3:			
		{
			// Отримання значення з таймера та скидання його значення
			time = timer.getElapsedTime().asMicroseconds();
			timer.restart();
			time /= 800;

			// Оброблення подій гри
			state = game->event_obj();

			// Оновлення даних гри
			game->update(time);

			// Малювання
			game->draw();

			// Якщо стан програми змінився на 2(головне меню)
			if (state == 2)
			{
				// Видаляємо гру та створюємо меню
				create_main_menu();
				delete_game();
			}
			
			// Якщо стан змінився на кінець гри(7)
			if (state == 7)
			{
				// Створюємо підвікно про результат гри
				create_end_game();
			}
			break;
		}

		case 4:
		{
			// Очистка вікна та малювання головного меню
			window->clear();
			menu->draw();

			// Запуск обробки подій підвікна мануала та його малювання
			state = additional_window->events_obj();
			additional_window->draw();

			// Якщо стан змінюється на 2(головне меню) видаляємо вікно мануала
			if (state == 2)
			{
				delete_manual();
			}
			break;
		}


		case 5:
		{
			// Очистка вікна та малювання головного меню
			window->clear();
			menu->draw();

			// Запуск обробки подій підвікна about та його малювання
			state = additional_window->events_obj();
			additional_window->draw();
			
			// Якщо стан змінюється на 2(головне меню) видаляємо вікно мануала
			if (state == 2)
			{
				delete_about();
			}
			break;
		}

		// Вихід з гри через кнопку
		case 6:
		{
			// Видалення гри та головного меню якщо вони є
			delete_main_menu();
			delete_game();
			
			// Закриття вікна 
			window->close();
			break;
		}

		case 7:
		{
			// Отримання значення з таймера та скидання його значення
			time = timer.getElapsedTime().asMicroseconds();
			timer.restart();
			time /= 800;

			// Оброблення подій гри
			game->update(time);

			// Рисование
			game->draw();

			// Малювання та обробка подій підвікна результату
			state = additional_window->events_obj();
			additional_window->draw();

			// Перехід до головного меню
			if (state == 2)
			{
				delete_end_game();
				delete_game();

				create_main_menu();
			}

			break;
		}

		}

		// Відображення вікна
		window->display();
	}
}

// Ініціалізація даних та створення вступної заставки
void MyWindow::create_intro()
{
	// Створення заствки та задання внутрішнього стану
	menu = new Menu(window, &s_click);
	menu->set_jump_state(1);

	// Задання фонового малюнку
	Texture* t = new Texture;
	t->loadFromFile(dir_background_intro);
	menu->set_background(t);

	// Ініціалізація текстів
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

	// Ініціалізація та запуск фонової музики
	fone.openFromFile(dir_intro_s);
	fone.setVolume(70);
	fone.play();
}

// Видалення вступної заставки
void MyWindow::delete_intro()
{
	if (menu)
		delete menu;
	menu = nullptr;
}

// Ініціалізація даних та створення головного меню
void MyWindow::create_main_menu()
{
	// Створення головного меню
	menu = new Menu(window, &s_click);
	menu->set_jump_state(2);

	// Задання фонового малюнку
	Texture* t = new Texture;
	t->loadFromFile(dir_background_intro);
	menu->set_background(t);

	// Ініціалізація текстів
	Draw_text* t1 = new Draw_text(window);
	t1->set_color(sf::Color::White);
	t1->set_size(40);
	t1->set_text("V1.0");
	t1->set_position(1200, 750);
	menu->add_Draw_text(t1);

	// Ініціалізація кнопок
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

	// Запуск фонової музики
	if (fone.getStatus() != sf::SoundSource::Status::Playing)
		fone.play();
}

// Видалення головного меню
void MyWindow::delete_main_menu()
{
	if (menu)
		delete menu;
	menu = nullptr;
}

// Створення та ініціалізація гри
void MyWindow::create_game()
{
	game = new Game(window);
	game->start();
}

// Видалення гри
void MyWindow::delete_game()
{
	if (game)
		delete game;
	game = nullptr;

}

// Створення підвікна інструкції до гри
void MyWindow::create_manual()
{
	// Створення підвікна
	additional_window = new Menu(window, &s_click);
	additional_window->set_jump_state(4);

	// Задання фонового малюнку
	Texture* texture = new Texture;
	texture->loadFromFile(dir_background_additional_window);
	additional_window->set_background(texture);

	// Ініціалізація текстів
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

	// Ініціалізація кнопок
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

// Видалення інструкції до гри
void MyWindow::delete_manual()
{
	if (additional_window)
		delete additional_window;
	additional_window = nullptr;
}

// Створення підвікна about 
void MyWindow::create_about()
{
	// Створення підвікна
	additional_window = new Menu(window, &s_click);
	additional_window->set_jump_state(5);

	// Задання фонового малюнку
	Texture* texture = new Texture;
	texture->loadFromFile(dir_background_additional_window);
	additional_window->set_background(texture);

	// Ініціалізація текстів
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

	// Ініціалізація кнопок
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

// Видалення підвікна about 
void MyWindow::delete_about()
{
	if (additional_window)
		delete additional_window;
	additional_window = nullptr;
}

// Створення підвікна про кінець гри
void MyWindow::create_end_game()
{
	// Створення підвікна
	additional_window = new Menu(window, &s_click);
	additional_window->set_jump_state(7);

	// Задання фонового малюнку
	Texture* texture = new Texture;
	texture->loadFromFile(dir_background_additional_window);
	additional_window->set_background(texture);

	// Отримання значень про результати гри
	int moves = game->get_moves();
	int failures = game->get_failures();
	int fines = game->get_fines();

	// Ініціалізація текстів
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

	// Ініціалізація кнопок
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

// Видалення підвікна результатів гри
void MyWindow::delete_end_game()
{
	if (additional_window)
		delete additional_window;
	additional_window = nullptr;
}