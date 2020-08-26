#pragma once
#include <engine.h>
#include "glm/gtx/rotate_vector.hpp"

class player
{
public:
	player();
	~player();

	void initialise(engine::ref<engine::game_object> object);
	void on_update(const engine::timestep& time_step);

	engine::ref<engine::game_object> object() const { return m_object; }

	void turn(float angle);
	

	//void jump();
	int get_score() { return score; };
	void set_score(int scr) { score = scr; };
	int get_health();
	void set_health(int n_health);
	bool get_invincible() { return invincible; }
	void set_invincible(bool invinc) {
		invincible = invinc;
	};
	void set_game_end(bool end) { game_end = end; }
private:
	float m_speed{ 0.f };
	int health;
	int score = 0;
	float fuel;
	bool invincible;
	engine::ref< engine::game_object> m_object;
	
	bool game_end = false;
};


