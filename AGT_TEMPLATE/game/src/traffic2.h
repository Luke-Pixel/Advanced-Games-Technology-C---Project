#pragma once
#include <engine.h>
#include "glm/gtx/rotate_vector.hpp"

class traffic2
{
public:
	traffic2();
	~traffic2();

	void initialise(engine::ref<engine::game_object> object, int d,int typ);
	void on_update(const engine::timestep& time_step);

	engine::ref<engine::game_object> object() const { return m_object; }

	
	
	std::string get_state() { return state; };
	void set_state(std::string st) { state = st; }
	
	

	
	float get_z();
;	int get_type() { return type; }
private:
	float m_speed{ 0.f };
	int type;
	std::string state;
	int direction;
	bool crashed = false;
	float crash_z;
	engine::ref< engine::game_object> m_object;
};

