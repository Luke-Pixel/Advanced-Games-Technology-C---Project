#include "pch.h"
#include "traffic2.h"
#include "engine/core/input.h"
#include "engine/key_codes.h"


traffic2::traffic2() :  m_speed(6.f)
{
}

traffic2::~traffic2()
{}

void traffic2::initialise(engine::ref<engine::game_object> object, int d, int typ)
{
	m_object = object;
	m_object->set_forward(glm::vec3(0.f, 0.f, 1.f));
	state = "Driving";
	direction = d;
	//m_object->set_mass(300.f);
	type = typ;
	//set the velocity of the cahrecter depending on direction chosen 
	if (state == "Driving" && direction == 1 && m_object->position().y <= 1.5f)
		m_object->set_velocity(-m_object->forward() * m_speed);
	else if (state == "Driving" && direction == 0)
		m_object->set_velocity(m_object->forward() * m_speed);

	
}

void traffic2::on_update(const engine::timestep& time_step)
{
	
	
	
		//turn(0.0f);
	/*CHange state of the AI depending on the position of the object in the game area*/
	if (m_object->position().x >= 2.8f && state != "Crashed") {
		if (crashed == false) {
			crash_z = get_z();
			crashed = true;
			
		}
		else if (crashed == true) {
			set_state("Stationary");
		}
		m_object->set_position(glm::vec3(2.8f, m_object->position().y, crash_z));
		if (state == "Driving")
			set_state("Crashed");
		
		//set_state("Stationary");
	}
	else if (m_object->position().x <= -2.8f && state != "Crashed") {
		if (crashed == false) {
			crash_z = get_z();
			crashed = true;
			
		}
		m_object->set_position(glm::vec3(-2.8f, m_object->position().y, m_object->position().z));
		if(state == "Driving")
			set_state("Crashed");

		
	}

	
	

}
//m ethod to return z cordinate fo object 
float traffic2::get_z() {
	return m_object->position().z;
}








