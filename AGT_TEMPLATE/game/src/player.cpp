#include "pch.h"
#include "player.h"
#include "engine/core/input.h"
#include "engine/key_codes.h"

player::player() :  m_speed(12.f), health(100.f)
{
}

player::~player()
{}
//initalise player with object, mass and forward direction 
void player::initialise(engine::ref<engine::game_object> object)
{
	m_object = object;
	m_object->set_mass(100.f);
	m_object->set_forward(glm::vec3(0.f, 0.f, 1.f));
	
}

void player::on_update(const engine::timestep& time_step)
{
	
	//set velocity to forward, multiplied by speed 
	m_object->set_velocity(- m_object->forward() * m_speed);
	//keyboard controls to control player 
	if (engine::input::key_pressed(engine::key_codes::KEY_1)) // left
		turn(1.0f * time_step);
	else if (engine::input::key_pressed(engine::key_codes::KEY_2)) // right
		turn(-1.0f * time_step);
	else
		turn(0.0f);
	//set x co ordinates so player dosent leave game area 
	if (m_object->position().x >= 2.8f) {
		m_object->set_position(glm::vec3(2.8f, m_object->position().y, m_object->position().z));
		
	}
	else if (m_object->position().x <= -2.8f) {
		m_object->set_position(glm::vec3(-2.8f, m_object->position().y, m_object->position().z));
		
	}

	
}

//set angular velocity to turn player charecter 
void player::turn(float angle)
{
	m_object->set_angular_velocity(glm::vec3(0.f, angle * 45.f, 0.f));
}


int player::get_health()
{
	return health;
}

void player::set_health(int n_health) {
	health = n_health;
}


