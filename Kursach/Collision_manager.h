#pragma once

#include <vector>
#include "shape_collision.h"
#include "Ball_collision.h"


// написати менеджер для обрахунку всіх колізій

// написати менеджер для обрахунку колізій шарів
using namespace std;

class manager_collision
{
	vector<shape_collision*> arr;

	bool collision_ball_ball(shape_collision* a, shape_collision* b);
	bool collision_ball_border(shape_collision* a, shape_collision* b);
	//bool collision_ball_hole(shape_collision* a, shape_collision* b);

public:
	manager_collision();

	bool add_collision(shape_collision* p);
	bool delete_collision(shape_collision* p);

	Ball_collision* search_ball_on_mouse() const;

	void update();
	
	~manager_collision();
};