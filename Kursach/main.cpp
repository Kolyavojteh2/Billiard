#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <cmath>
#include <vector>
#include "Ball_shape.h"
#include "Vector_rad.h"
#include "Select_Ball.h"
#include "MyWindow.h"
#include "Game.h"
#include <string>

using namespace std;
using namespace sf;

// ��������� ������ ����
const int window_width = 1300;
const int window_height = 800;

// ����� �� �������
const char* dir_button = "res/textures/button.png";
const char* dir_background_additional_window = "res/textures/background_window.png";
const char* dir_background_intro = "res/textures/intro.jpg";
const char* dir_ball = "res/textures/ball_32.png";
const char* dir_cue = "res/textures/cue.png";
const char* dir_select = "res/textures/select_rect.png";
const char* dir_power = "res/textures/power.png";
const char* dir_board = "res/textures/board_1200_670.png";
const char* dir_select_ball = "res/textures/select.png";

// ����� �� �����
const char* dir_error_s = "res/sounds/error.ogg";
const char* dir_ball_s = "res/sounds/ball.ogg";
const char* dir_border_s = "res/sounds/border.ogg";
const char* dir_cue_s = "res/sounds/cue.ogg";
const char* dir_hole_s = "res/sounds/hole.ogg";
const char* dir_intro_s = "res/sounds/intro.ogg";
const char* dir_fone_s = "res/sounds/fone.ogg";

// ����� �� ������
const char* dir_font = "res/fonts/times new roman.ttf";

int main()
{
	MyWindow* window = new MyWindow(VideoMode(window_width, window_height), "Billiard");

	// ������ ��������� ����� ��������
	window->event_handling();

	delete window;

	return 0;
}