#include <iostream>
#include <cmath>
#include "pch.h"
#include "traffic1.h"

traffic1::traffic1()
{}

traffic1::~traffic1()
{}

//initalises by seetting m_object to object parameter passed
void traffic1::initialise(engine::ref<engine::game_object> object)
{
	m_object = object;
}



//update method called every frame by example_layer
void traffic1::on_update(const engine::timestep& time_step)
{
	// Update physical quanitities

	m_last_position = m_object->position();
	//update my position to current position of object 
	m_my_position = m_object->position();
	//ai method is called to update velocirt of the AI
	ai_behaviour();	
}


void traffic1::on_render(const engine::ref<engine::shader>& shader)
{
	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, m_object->position());
	transform = glm::rotate(transform, m_object->rotation_amount(), m_object->rotation_axis());
	engine::renderer::submit(shader, transform, m_object);
}





//impleementation of a follow AI behaviour 
void traffic1::ai_behaviour() {
	if (seen == false) {
		//m_object->set_velocity(m_object->forward() * 2.f);

		//normalize vectors of negate of player position and AI position 
		desired_velocity = normalize(m_player_position - m_object->position());
		//set the steering to the 
		steering = desired_velocity - m_object->velocity();

		// divide sterringby mass of object 
		steering = steering / m_object->mass();
		//set the angualr velocity of the object to steering vector
		m_object->set_angular_velocity(steering);
		//add steering to velocity to move object towasrds player
		m_object->set_velocity((m_object->velocity() + steering) *7.f );
		//update object positon 
		m_object->position() = m_object->position() + m_object->velocity();
	}
}
